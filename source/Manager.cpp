#include "Manager.h"
#include <functional>
#include <iostream>
#include <ostream>

#include "Rules.h"
#include "World.h"
#include "Chunk.h"

Manager::Manager(World& world, Rules& rules)
    : world(world), rules(rules) {}

void Manager::find_active_neighbour(const std::array<long long, 8>& keys,
                                    const std::unordered_map<long long, Chunk>& selected)
{
    active_neighbour.clear();
    for (int id = 0 ; id < 8; ++id)
    {
        if (long long key = keys[id]; selected.contains(key))
            active_neighbour.emplace_back(id, key);
    }
}

void Manager::neighbours_cells_edge(const std::unordered_map<long long, Chunk>& selected_world, const int SIZE)
{
    neighbour_cells.clear();
    for (auto [id, neighbour_key] : active_neighbour) {
        auto& neighbour = const_cast<Chunk&>(selected_world.at(neighbour_key));
        std::vector<std::reference_wrapper<Cell>> cells;
        switch (id) {
        case 0: // N row: y=0, x=0..SIZE-1
            cells.reserve(SIZE);
            for (int x = 0; x < SIZE; ++x)
                cells.emplace_back(neighbour.get_cell(x, 0));
            break;
        case 1: // NE corner: (SIZE-1, 0)
            cells.emplace_back(neighbour.get_cell(SIZE-1, 0));
            break;
        case 2: // E col: x=SIZE-1, y=0     SIZE-1
            cells.reserve(SIZE);
            for (int y = 0; y < SIZE; ++y)
                cells.emplace_back(neighbour.get_cell(SIZE-1, y));
            break;
        case 3: // SE corner: (SIZE-1, SIZE-1)
            cells.emplace_back(neighbour.get_cell(SIZE-1, SIZE-1));
            break;
        case 4: // S row: y=SIZE-1, x=0     SIZE-1
            cells.reserve(SIZE);
            for (int x = 0; x < SIZE; ++x)
                cells.emplace_back(neighbour.get_cell(x, SIZE-1));
            break;
        case 5: // SW corner: (0, SIZE-1)
            cells.emplace_back(neighbour.get_cell(0, SIZE-1));
            break;
        case 6: // W col: x=0, y=0          SIZE-1
            cells.reserve(SIZE);
            for (int y = 0; y < SIZE; ++y)
                cells.emplace_back(neighbour.get_cell(0, y));
            break;
        case 7: // NW corner: (0, 0)
            cells.emplace_back(neighbour.get_cell(0, 0));
            break;
        default: break;
        }
        neighbour_cells.emplace_back(id, std::move(cells));
    }
}

void Manager::halo_bridge(Chunk& buffer,
    const std::vector<std::pair<int, std::vector<std::reference_wrapper<Cell>>>>& cells,
    const int size, const haloDirection dir)
{
    if (cells.empty()) return;
    for (auto [id, cellRef] : cells)
    {
        switch (id)
        {
        case 0: // North (top row)
            {
                for (int x = 0; x < size; ++x)
                {
                    if (dir == haloDirection::Import)
                        buffer.get_cell(x+1, 0).set_type(cellRef[x].get().get_type());
                    else
                        cellRef.at(x).get().set_type(buffer.get_cell(x+1, 0).get_type());
                }
                break;
            }
        case 1: // North-East (top-right corner)
            {
                if (dir == haloDirection::Import)
                    buffer.get_cell(size + 1, 0).set_type(cellRef[0].get().get_type());
                else
                    cellRef[0].get().set_type(buffer.get_cell(size +1, 0).get_type());
                break;
            }
        case 2: // East (right column)
            {
                for (int y = 0; y < size; ++y)
                {
                    if (dir == haloDirection::Import)
                        buffer.get_cell(size+1, y+1).set_type(cellRef[y].get().get_type());
                    else
                        cellRef[y].get().set_type(buffer.get_cell(size+1, y+1).get_type());
                }
                break;
            }
        case 3: // South-East (bottom-right corner)
            {
                if (dir == haloDirection::Import)
                    buffer.get_cell(size + 1, size + 1).set_type(cellRef[0].get().get_type());
                else
                    cellRef[0].get().set_type(buffer.get_cell(size + 1, size + 1).get_type());
                break;
            }
        case 4: // South (bottom row)
            {
                for (int x = 0; x < size; ++x)
                {
                    if (dir == haloDirection::Import)
                        buffer.get_cell(x+1, size + 1).set_type(cellRef[x].get().get_type());
                    else
                        cellRef[x].get().set_type(buffer.get_cell(x+1, size+1).get_type());
                }
                break;
            }
        case 5: // South-West (bottom-left corner)
            {
                if (dir == haloDirection::Import)
                    buffer.get_cell(0, size + 1).set_type(cellRef[0].get().get_type());
                else
                    cellRef[0].get().set_type(buffer.get_cell(0, size+1).get_type());
                break;
            }
        case 6: // West (left column)
            {
                for (int y = 0; y < size; ++y)
                {
                    if (dir == haloDirection::Import)
                        buffer.get_cell(0, y+1).set_type(cellRef[y].get().get_type());
                    else
                        cellRef[y].get().set_type(buffer.get_cell(0, y+1).get_type());
                }
                break;
            }
        case 7: // North-West (top-left corner)
            {
                if (dir == haloDirection::Import)
                    buffer.get_cell(0, 0).set_type(cellRef[0].get().get_type());
                else
                    cellRef[0].get().set_type(buffer.get_cell(0, 0).get_type());
                break;
            }
        default: break;
        }
    }
}

void Manager::construct_halo(Chunk& buffer, Chunk& selected,
    const std::vector<std::pair<int, std::vector<std::reference_wrapper<Cell>>>>& cells)
{
    const int size = selected.get_size(); // inner: 3
    constexpr int halo = 1;
    // copy inner into buffer at offset (1size maps to buffer)
    for (int y = 0; y < size; ++y)
        for (int x = 0; x < size; ++x)
            buffer.get_cell(x + halo, y + halo).set_type(selected.get_cell(x, y).get_type());
    halo_bridge(buffer, cells, size, haloDirection::Import);
}

void Manager::chunk_update(Chunk& buffer, Chunk& selected)
{
    const int size = selected.get_size(); // 3
    for (int y = 1; y <= size; ++y) {
        for (int x = 1; x <= size; ++x) {
            const int live = buffer.neighbour_count(x, y); // live neighbour count
            const bool current_cell = buffer.get_cell(x, y).is_alive(); // if current cell is alive
            Cell& target = selected.get_cell(x-1, y-1); // shifted x|y by 1 for other index
            // rules
            if (current_cell && (live < 2 || live > 3)) {
                target.set_type('0');   // death
            } else if (!current_cell && live == 3) {
                target.set_type('w');   // birth
            } else {
                target.set_type(buffer.get_cell(x, y).get_type());
            }
        }
    }
}

void Manager::update()
{
    auto& world_data = world.get_world();
    for (auto&[key, chunk] : world_data)
    {
        const int size = chunk.get_size();
        // enforce fixed neighbour order
        std::array<long long, 8> keys = world.get_neighbour_key(key);

        // find active neighbours
        find_active_neighbour(keys, world_data);

        // buffer with halo
        Chunk buffer(chunk.get_CX(), chunk.get_CY(), chunk.get_size() + 2);

        // import halos
        if (!active_neighbour.empty())
            neighbours_cells_edge(world_data, size);

        construct_halo(buffer, chunk, neighbour_cells);

        // update inner cells
        chunk_update(buffer, chunk);

        // export halos back
        if (!active_neighbour.empty())
            halo_bridge(buffer, neighbour_cells, size, haloDirection::Export);
    }
}

void Manager::step()
{

}

void Manager::render_step()
{

}