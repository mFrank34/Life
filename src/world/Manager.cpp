#include "Manager.h"
#include "rules/Rules.h"
#include "world/World.h"
#include "world/structure/Chunk.h"

#include <functional>
#include <iostream>
#include <map>
#include <ostream>
#include <ranges>

void Manager::find_neighbour(
    const std::array<long long, 8>& keys,
    const std::unordered_map<long long, Chunk>& map)
{
    active_neighbour.clear();
    missing_neighbour.clear();
    for (int id = 0; id < 8; ++id)
    {
        if (long long key = keys[id]; map.contains(key))
            active_neighbour.emplace_back(id, key);
        else
            missing_neighbour.emplace_back(id, key);
    }
}

void Manager::generate_missing_neighbour()
{
    for (auto [id, key] : missing_neighbour)
    {
        world->get_chunk(key);
        active_neighbour.emplace_back(id, key);
    }
}

void Manager::get_neighbours_edge_case(
    std::unordered_map<long long, Chunk>& selected_world,
    const int SIZE)
{
    neighbour_cells.clear();
    for (auto [id, neighbour_key] : active_neighbour)
    {
        auto& neighbour = selected_world.at(neighbour_key);
        std::vector<std::reference_wrapper<Cell>> cells;
        switch (id)
        {
        case 0: // N (take neighbor's bottom row)
            for (int x = 0; x < SIZE; ++x)
                cells.emplace_back(neighbour.get_cell(x, SIZE - 1));
            break;
        case 1: // NE (neighbor bottom-left corner)
            cells.emplace_back(neighbour.get_cell(0, SIZE - 1));
            break;
        case 2: // E (take neighbour's left column)d
            for (int y = 0; y < SIZE; ++y)
                cells.emplace_back(neighbour.get_cell(0, y));
            break;
        case 3: // SE (neighbor top-left corner)
            cells.emplace_back(neighbour.get_cell(0, 0));
            break;
        case 4: // S (take neighbor's top row)
            for (int x = 0; x < SIZE; ++x)
                cells.emplace_back(neighbour.get_cell(x, 0));
            break;
        case 5: // SW (neighbor top-right corner)
            cells.emplace_back(neighbour.get_cell(SIZE - 1, 0));
            break;
        case 6: // W (take neighbor’s right column)
            for (int y = 0; y < SIZE; ++y)
                cells.emplace_back(neighbour.get_cell(SIZE - 1, y));
            break;
        case 7: // NW (neighbor bottom-right corner)
            cells.emplace_back(neighbour.get_cell(SIZE - 1, SIZE - 1));
            break;
        default: break;
        }
        neighbour_cells.emplace_back(id, std::move(cells));
    }
}

void Manager::halo_bridge(Chunk& buffer,
                          const std::vector<std::pair<int, std::vector<std::reference_wrapper<Cell>>>>& cells,
                          const int size,
                          const haloDirection dir)
{
    const std::map<int, haloInfo> halo_map = {
        // id, {startX, startY, stepX, stepY, count}
        {0, {1, 0, 1, 0, size}}, // 0: North (top row)
        {1, {size + 1, 0, 0, 0, 1}}, // 1: North-East (top-right corner)
        {2, {size + 1, 1, 0, 1, size}}, // 2: East (right column)
        {3, {size + 1, size + 1, 0, 0, 1}}, // 3: South-East (bottom-right corner)
        {4, {1, size + 1, 1, 0, size}}, // 4: South (bottom row)
        {5, {0, size + 1, 0, 0, 1}}, // 5: South-West (bottom-left corner)
        {6, {0, 1, 0, 1, size}}, // 6: West (left column)
        {7, {0, 0, 0, 0, 1}} // 7: North-West (top-left corner)
    };

    for (auto& [id, cellRef] : cells)
    {
        auto it = halo_map.find(id);
        if (it == halo_map.end()) continue;

        const haloInfo& halo = it->second;
        for (int i = 0; i < halo.count; ++i)
        {
            const int bx = halo.startX + halo.stepX * i;
            const int by = halo.startY + halo.stepY * i;

            Cell& halo_call = buffer.get_cell(bx, by);
            Cell& remote_cell = cellRef.at(i).get();

            if (dir == haloDirection::Import)
                halo_call.set_type(remote_cell.get_type());
            else
                remote_cell.set_type(halo_call.get_type());
        }
    }
}

void Manager::construct_halo(Chunk& buffer, Chunk& selected,
                             const std::vector<std::pair<int, std::vector<std::reference_wrapper<Cell>>>>& cells)
{
    const int size = selected.get_size();
    constexpr int halo = 1;
    // copy inner into buffer at offset (1size maps to buffer)
    for (int y = 0; y < size; ++y)
        for (int x = 0; x < size; ++x)
            buffer.get_cell(x + halo, y + halo).set_type(selected.get_cell(x, y).get_type());
    halo_bridge(buffer, cells, size, haloDirection::Import);
}

void Manager::chunk_update(Chunk& haloBuffer,
                           Chunk& next,
                           const Chunk& current)
{
    const int size = current.get_size();

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            const int bx = x + 1;
            const int by = y + 1;

            const int live = haloBuffer.neighbour_count(bx, by);
            const bool alive = haloBuffer.get_cell(bx, by).is_alive();

            Cell& out = next.get_cell(x, y);

            if (alive && (live < 2 || live > 3))
                out.set_type('0');
            else if (!alive && live == 3)
                out.set_type('w');
            else
                out.set_type(haloBuffer.get_cell(bx, by).get_type());
        }
    }
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
    auto& next_data = world->get_next_world();

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

        get_neighbours_edge_case(world_data, size);

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
