/*
 * File: Manager.cpp
 * Author: Michael Franks
 * Description: updates the world; thread-safe implementation.
 *
 * Thread safety strategy
 * ─────────────────────
 * All access to the shared World (get_chunk, get_size, map iteration) is
 * confined to the main thread in a pre-phase.  Worker threads receive a
 * self-contained ChunkWork value that owns every piece of data it needs,
 * so no synchronization is required inside the hot path.
 */

#include "Manager.h"

#include <algorithm>

#include "profiler/Profiler.h"
#include "rules/Rules.h"
#include "world/World.h"
#include "world/structure/Chunk.h"

std::vector<Manager::Neighbour> Manager::find_neighbour(
    const std::array<long long, 8>& keys,
    const std::unordered_map<long long, Chunk>& map
)
{
    std::vector<Neighbour> neighbours;
    neighbours.reserve(8);

    for (int i = 0; i < 8; ++i)
    {
        Cardinal dir = static_cast<Cardinal>(i);
        long long key = keys[i];

        // Ensure the chunk exists in the world (main thread only).
        if (!map.contains(key))
            world->get_chunk(key);

        neighbours.push_back({dir, key});
    }
    return neighbours;
}

std::vector<Manager::NeighbourCell> Manager::get_edge_case(
    const std::vector<Neighbour>& neighbours,
    const int SIZE
)
{
    std::vector<NeighbourCell> neighbour_cells;
    neighbour_cells.reserve(neighbours.size());

    for (const auto& [dir, key] : neighbours)
    {
        // All world access happens here on the main thread.
        const Chunk& neighbour = world->get_chunk(key);
        std::vector<Cell> cells; // ← owned copies, not references

        switch (dir)
        {
        case Cardinal::N:
            cells.reserve(SIZE);
            for (int x = 0; x < SIZE; ++x)
                cells.push_back(neighbour.get_cell(x, SIZE - 1));
            break;

        case Cardinal::NE:
            cells.push_back(neighbour.get_cell(0, SIZE - 1));
            break;

        case Cardinal::E:
            cells.reserve(SIZE);
            for (int y = 0; y < SIZE; ++y)
                cells.push_back(neighbour.get_cell(0, y));
            break;

        case Cardinal::SE:
            cells.push_back(neighbour.get_cell(0, 0));
            break;

        case Cardinal::S:
            cells.reserve(SIZE);
            for (int x = 0; x < SIZE; ++x)
                cells.push_back(neighbour.get_cell(x, 0));
            break;

        case Cardinal::SW:
            cells.push_back(neighbour.get_cell(SIZE - 1, 0));
            break;

        case Cardinal::W:
            cells.reserve(SIZE);
            for (int y = 0; y < SIZE; ++y)
                cells.push_back(neighbour.get_cell(SIZE - 1, y));
            break;

        case Cardinal::NW:
            cells.push_back(neighbour.get_cell(SIZE - 1, SIZE - 1));
            break;
        }

        neighbour_cells.push_back({dir, std::move(cells)});
    }
    return neighbour_cells;
}

std::array<Manager::HaloMap, 8> Manager::build_table(int size)
{
    // Built fresh each call so it correctly reflects the current size.
    return {
        {
            {Cardinal::N, {1, 0, 1, 0, size}},
            {Cardinal::NE, {size + 1, 0, 0, 0, 1}},
            {Cardinal::E, {size + 1, 1, 0, 1, size}},
            {Cardinal::SE, {size + 1, size + 1, 0, 0, 1}},
            {Cardinal::S, {1, size + 1, 1, 0, size}},
            {Cardinal::SW, {0, size + 1, 0, 0, 1}},
            {Cardinal::W, {0, 1, 0, 1, size}},
            {Cardinal::NW, {0, 0, 0, 0, 1}}
        }
    };
}

Chunk Manager::build_halo(
    Chunk& selected,
    const std::vector<NeighbourCell>& neighbours,
    int size
)
{
    constexpr int halo = 1;

    Chunk buffer(selected.get_CX(), selected.get_CY(), size + CHUNK_OFF_SET);

    // 1. Copy inner chunk into the halo buffer.
    for (int y = 0; y < size; ++y)
        for (int x = 0; x < size; ++x)
            buffer.get_cell(x + halo, y + halo)
                  .set_type(selected.get_cell(x, y).get_type());

    // 2. Build the halo lookup table for this size (not static — size varies).
    const std::array<HaloMap, 8> halo_table = build_table(size);

    // 3. Import neighbour border cells into the halo fringe.
    for (const NeighbourCell& neighbour : neighbours)
    {
        auto it = std::find_if(
            halo_table.begin(),
            halo_table.end(),
            [&](const HaloMap& map)
            {
                return map.direction == neighbour.direction;
            }
        );

        if (it == halo_table.end())
            continue;

        const HaloTable& h = it->table;

        for (int i = 0; i < h.count; ++i)
        {
            const int bx = h.startX + h.stepX * i;
            const int by = h.startY + h.stepY * i;

            // neighbour.cells holds owned Cell copies — no shared state.
            buffer.get_cell(bx, by)
                  .set_type(neighbour.cells[i].get_type());
        }
    }
    return buffer;
}

Chunk Manager::chunk_update(const Chunk& halo, int size)
{
    Chunk next(halo.get_CX(), halo.get_CY(), size);

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            const int bx = x + 1;
            const int by = y + 1;

            const Cell& current = halo.get_cell(bx, by);
            const CellType type = current.get_type();
            Cell& out = next.get_cell(x, y);
            const Count count = halo.neighbour_count(bx, by);

            if (current.is_alive())
            {
                switch (type)
                {
                case CellType::White:
                    out.set_type(rules.white.survives(count.white) ? CellType::White : CellType::Empty);
                    break;
                case CellType::Blue:
                    out.set_type(rules.blue.survives(count.blue) ? CellType::Blue : CellType::Empty);
                    break;
                case CellType::Green:
                    out.set_type(rules.green.survives(count.green) ? CellType::Green : CellType::Empty);
                    break;
                case CellType::Red:
                    out.set_type(rules.red.survives(count.red) ? CellType::Red : CellType::Empty);
                    break;
                default:
                    out.set_type(CellType::Empty);
                    break;
                }
            }
            else
            {
                if (rules.white.births(count.white)) out.set_type(CellType::White);
                else if (rules.blue.births(count.blue)) out.set_type(CellType::Blue);
                else if (rules.green.births(count.green)) out.set_type(CellType::Green);
                else if (rules.red.births(count.red)) out.set_type(CellType::Red);
                else out.set_type(CellType::Empty);
            }
        }
    }
    return next;
}

Manager::Manager(Rules& rules, Scheduler& scheduler)
    : rules(rules), scheduler(scheduler)
{
}

void Manager::attach_world(World& world)
{
    this->world = &world;
}

void Manager::update()
{
    // Don't start a new tick if the previous one hasn't finished
    if (updating.load()) return;
    updating = true;

    Perf::Profiler::get().begin_update();

    auto& world_data = world->get_world();

    /* --- Phase 0: ensure all neighbour chunks exist --- */
    {
        std::vector<long long> to_create;

        for (auto& [key, chunk] : world_data)
            for (long long nkey : world->get_neighbour_key(key))
                if (!world_data.contains(nkey))
                    to_create.push_back(nkey);

        for (long long key : to_create)
            world->get_chunk(key);
    }

    /* --- Phase 0.5: collect all work on the main thread --- */
    const int size = world->get_size();

    auto work_items = std::make_shared<std::vector<ChunkWork>>();
    work_items->reserve(world_data.size());

    for (auto& [key, chunk] : world_data)
    {
        // Skip chunks with no alive cells AND no populated neighbours
        if (!chunk.is_populated())
        {
            bool has_populated_neighbour = false;
            for (long long nkey : world->get_neighbour_key(key))
            {
                auto it = world_data.find(nkey);
                if (it != world_data.end() && it->second.is_populated())
                {
                    has_populated_neighbour = true;
                    break;
                }
            }
            if (!has_populated_neighbour) continue;
        }

        auto neighbour_keys = world->get_neighbour_key(key);
        auto neighbours = find_neighbour(neighbour_keys, world_data);
        auto neighbour_cells = get_edge_case(neighbours, size);

        work_items->push_back({key, chunk, std::move(neighbour_cells), size});
    }

    if (work_items->empty())
    {
        updating = false;
        return;
    }

    /* --- Phase 1: parallel update — no shared state in hot path --- */
    auto results = std::make_shared<std::vector<std::pair<long long, Chunk>>>();
    auto results_mtx = std::make_shared<std::mutex>();
    auto remaining = std::make_shared<std::atomic<int>>((int)work_items->size());
    auto world_ptr = world;

    results->reserve(work_items->size());

    for (auto& work : *work_items)
    {
        scheduler.enqueue_grouped(
            [this, work = std::move(work), results, results_mtx, remaining, world_ptr]() mutable
            {
                Chunk new_chunk = chunk_update(
                    build_halo(work.chunk_copy, work.neighbour_cells, work.size),
                    work.size
                );

                {
                    std::lock_guard lock(*results_mtx);
                    results->emplace_back(work.key, std::move(new_chunk));
                }
                // Last thread to finish commits results and swaps
                if (remaining->fetch_sub(1) == 1)
                {
                    {
                        std::lock_guard lock(*results_mtx);
                        auto& next_step = world_ptr->get_stepped_world();

                        // Commit updated chunks
                        for (auto& [key, chunk] : *results)
                            next_step.insert_or_assign(key, std::move(chunk));

                        // Carry over any chunks that were skipped (empty, not processed)
                        // so they don't vanish after the swap
                        auto& world_data = world_ptr->get_world();
                        for (auto& [key, chunk] : world_data)
                            if (!next_step.contains(key))
                                next_step.insert_or_assign(key, chunk);
                    }

                    {
                        std::unique_lock lock(world_ptr->world_mtx);
                        world_ptr->swap_world();
                    }

                    world_ptr->unload();

                    // Record profiler stats
                    Perf::Profiler::get().end_update();
                    auto& wd = world_ptr->get_world();
                    int chunk_size = world_ptr->get_size();
                    unsigned long mem = wd.size() * chunk_size * chunk_size * sizeof(uint8_t);
                    Perf::Profiler::get().record((int)wd.size(), mem);

                    updating = false;
                }
            }
        );
    }
}

bool Manager::is_updating()
{
    return updating;
}
