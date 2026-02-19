/*
 * File: Manager.cpp
 * Author: Michael Franks
 * Description: updates the world and system need to update a system
 */

#include "Manager.h"

#include <algorithm>

#include "rules/Rules.h"
#include "world/World.h"
#include "world/structure/Chunk.h"

#include <functional>
#include <map>
#include <ranges>

std::vector<Manager::Neighbour> Manager::find_neighbour(
    const std::array<long long, 8>& keys,
    const std::unordered_map<long long, Chunk>& map
)
{
    std::vector<Neighbour> neighbours;
    for (int i = 0; i < 8; ++i)
    {
        Cardinal dir = static_cast<Cardinal>(i);
        long long key = keys[i];

        if (!map.contains(key))
            world->get_chunk(key);

        neighbours.emplace_back(dir, key);
        neighbours.emplace_back(dir, key);
    }
    return neighbours;
}

std::vector<Manager::NeighbourCell> Manager::get_edge_case(
    std::vector<Neighbour> neighbours,
    const int SIZE
)
{
    std::vector<NeighbourCell> neighbour_cells;
    for (auto [dir, key] : neighbours)
    {
        auto& neighbour = world->get_chunk(key);
        std::vector<std::reference_wrapper<Cell>> cells;

        switch (dir)
        {
        case Cardinal::N:
            for (int x = 0; x < SIZE; ++x)
                cells.emplace_back(neighbour.get_cell(x, SIZE - 1));
            break;
        case Cardinal::NE:
            cells.emplace_back(neighbour.get_cell(0, SIZE - 1));
            break;
        case Cardinal::E:
            for (int y = 0; y < SIZE; ++y)
                cells.emplace_back(neighbour.get_cell(0, y));
            break;
        case Cardinal::SE:
            cells.emplace_back(neighbour.get_cell(0, 0));
            break;
        case Cardinal::S:
            for (int x = 0; x < SIZE; ++x)
                cells.emplace_back(neighbour.get_cell(x, 0));
            break;
        case Cardinal::SW:
            cells.emplace_back(neighbour.get_cell(SIZE - 1, 0));
            break;
        case Cardinal::W:
            for (int y = 0; y < SIZE; ++y)
                cells.emplace_back(neighbour.get_cell(SIZE - 1, y));
            break;
        case Cardinal::NW:
            cells.emplace_back(neighbour.get_cell(SIZE - 1, SIZE - 1));
            break;
        }

        neighbour_cells.emplace_back(dir, std::move(cells));
    }
    return neighbour_cells;
}

std::array<Manager::HaloMap, 8> Manager::build_table(int size)
{
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
    const std::vector<NeighbourCell>& neighbours
)
{
    const int size = world->get_size();
    constexpr int halo = 1;

    Chunk buffer(selected.get_CX(), selected.get_CY(), size + CHUNK_OFF_SET);

    // 1. Copy inner chunk
    for (int y = 0; y < size; ++y)
        for (int x = 0; x < size; ++x)
            buffer.get_cell(x + halo, y + halo)
                  .set_type(selected.get_cell(x, y).get_type());

    // 2. Build halo lookup table
    static std::array<HaloMap, 8> halo_table = build_table(size);

    // 3. Import neighbour cells
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

            buffer.get_cell(bx, by)
                  .set_type(neighbour.cells[i].get().get_type());
        }
    }
    return buffer;
}

Chunk Manager::chunk_update(const Chunk& halo)
{
    const int size = world->get_size();
    Chunk next(halo.get_CX(), halo.get_CY(), size);

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            const int bx = x + 1;
            const int by = y + 1;

            const int live = halo.neighbour_count(bx, by);
            const bool alive = halo.get_cell(bx, by).is_alive();

            Cell& out = next.get_cell(x, y);

            if (alive && (live < 2 || live > 3))
                out.set_type('0');
            else if (!alive && live == 3)
                out.set_type('w');
            else
                out.set_type(halo.get_cell(bx, by).get_type());
        }
    }
    return next;
}


Manager::Manager(Rules& rules)
    : rules(rules)
{
}

void Manager::attach_world(World& world)
{
    this->world = &world;
}

void Manager::update()
{
    auto& world_data = world->get_world();
    auto& next_data = world->get_stepped_world();

    // --- Phase 0: Ensure Sync between both worlds
    std::vector<long long> to_create;

    for (auto& [key, chunk] : world_data)
    {
        auto keys = world->get_neighbour_key(key);
        for (long long nkey : keys)
        {
            if (!world_data.contains(nkey))
                to_create.push_back(nkey);
        }
    }

    for (long long key : to_create)
        world->get_chunk(key);

    // --- Phase 1: compute next stage of game ---

    for (auto& [key, chunk] : world_data)
    {
        const int size = chunk.get_size();

        auto keys = world->get_neighbour_key(key);
        find_neighbour(keys, world_data);

        get_edge_case(world_data, size);

        Chunk halo(chunk.get_CX(), chunk.get_CY(), size + 2);
        construct_halo(halo, chunk, neighbour_cells);

        // ensure nextWorld chunk exists
        if (!next_data.contains(key))
        {
            next_data.try_emplace(
                key,
                chunk.get_CX(),
                chunk.get_CY(),
                size
            );
        }

        Chunk& next = next_data.at(key);
        chunk_update(halo, next, chunk);
    }

    // --- Phase 2: commit changes ---
    world->swap_world();
    world->unload();
}
