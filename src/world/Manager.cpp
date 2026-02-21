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

// TODO: make fixes too this
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
            const bool alive = current.is_alive();
            const CellType type = current.get_type();

            Cell& out = next.get_cell(x, y);

            // Count neighbors by color
            Count count = halo.neighbour_count(bx, by);
            int totalAlive = count.blue + count.green + count.red + count.white;

            if (alive)
            {
                // Survival per color
                switch (type)
                {
                case CellType::White:
                    if (totalAlive < 2 || totalAlive > 3)
                        out.set_type(CellType::Empty);
                    else
                        out.set_type(CellType::White);
                    break;
                case CellType::Blue:
                    if (count.blue != 4)
                        out.set_type(CellType::Empty);
                    else
                        out.set_type(CellType::Blue);
                    break;
                case CellType::Green:
                    if (count.green != 6)
                        out.set_type(CellType::Empty);
                    else
                        out.set_type(CellType::Green);
                    break;
                case CellType::Red:
                    if (count.red != 8)
                        out.set_type(CellType::Empty);
                    else
                        out.set_type(CellType::Red);
                    break;
                default:
                    out.set_type(CellType::Empty);
                    break;
                }
            }
            else
            {
                // Dead cell — check for birth
                if (totalAlive == 3)
                {
                    // Classic Conway birth for white
                    out.set_type(CellType::White);
                }
                else if (count.blue == 5)
                {
                    out.set_type(CellType::Blue);
                }
                else if (count.green == 7)
                {
                    out.set_type(CellType::Green);
                }
                else if (count.red == 9)
                {
                    out.set_type(CellType::Red);
                }
                else
                {
                    out.set_type(CellType::Empty); // stays dead
                }
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
    auto& world_data = world->get_world();
    auto& next_step = world->get_stepped_world();

    /* --- Phase 0: ensure all neighbour chunks exist --- */
    {
        std::vector<long long> to_create;

        for (auto& [key, chunk] : world_data)
        {
            for (long long nkey : world->get_neighbour_key(key))
                if (!world_data.contains(nkey))
                    to_create.push_back(nkey);
        }

        for (long long key : to_create)
            world->get_chunk(key);
    }

    /* --- Phase 0.5: collect all work on the main thread ---
     *  find_neighbor / get_edge_case both touch the World.  We do this
     *  entirely before any threads are spawned so that:
     *    • no concurrent mutation of world_data can occur, and
     *    • the ChunkWork values passed to threads own their data outright.
     */
    const int size = world->get_size(); // snapshot once

    std::vector<ChunkWork> work_items;
    work_items.reserve(world_data.size());

    for (auto& [key, chunk] : world_data)
    {
        auto neighbour_keys = world->get_neighbour_key(key);
        auto neighbours = find_neighbour(neighbour_keys, world_data);
        auto neighbour_cells = get_edge_case(neighbours, size);

        work_items.push_back({
            key,
            chunk, // copy (snapshot)
            std::move(neighbour_cells),
            size
        });
    }

    /* --- Phase 1: parallel update — threads touch no shared state --- */
    results.reserve(work_items.size());

    for (auto& work : work_items)
    {
        scheduler.enqueue_grouped(
            // Capture work by value so each lambda is fully self-contained.
            [this, work = std::move(work)]() mutable
            {
                Chunk new_chunk =
                    chunk_update(
                        build_halo(
                            work.chunk_copy,
                            work.neighbour_cells,
                            work.size
                        ),
                        work.size
                    );

                {
                    std::lock_guard<std::mutex> lock(result_mtx);
                    results.emplace_back(work.key, std::move(new_chunk));
                }
            }
        );
    }

    /* --- Phase 1.1: wait for all workers --- */
    scheduler.wait_for_group();

    /* --- Phase 1.2: commit thread results --- */
    for (auto& [key, chunk] : results)
        next_step.insert_or_assign(key, std::move(chunk));

    results.clear();

    /* --- Phase 2: swap and unload --- */
    world->swap_world();
    world->unload();
}