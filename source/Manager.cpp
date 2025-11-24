#include "Manager.h"

#include "Rules.h"
#include "World.h"
#include "Chunk.h"

Manager::Manager(World& world, Rules& rules)
    : world(world), rules(rules) {}

void Manager::find_active_neighbour(const std::vector<long long>& keys,
    const std::unordered_map<long long, Chunk>& selected)
{
    for (long long neighbour_key : keys)
    {
        /* Chunks lay out: starting from 0 to 7 for 8 entries
         * 1 2 3    * is chunk that being updated
         * 4 * 5    - need to find out chunks that are
         * 6 7 8    - alive within world
         */
        int id = 0;
        if (selected.contains(neighbour_key))
        {
            active_neighbour.emplace_back(id, neighbour_key);
        }
        ++id;
    }
}

void Manager::neighbours_cells_edge(std::unordered_map<long long, Chunk> selected_world, int SIZE)
{
    for (auto[id, neighbour_key] : active_neighbour)
    {
        std::vector<Cell> cells;
        switch (id) // id is number around chunks
        {
        case 0:
            {   // 1 cell every time
                Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                const int index = (SIZE * SIZE) - 1; // working maths backward for indexing
                cells.emplace_back(selected_neighbour.get_cell(index));
                neighbour_cells.emplace_back(id,cells);
                break;
            }
        case 1:
            {   // multi-able cells depending on size
                Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                for (int i = 1; i < SIZE; i++)
                {
                    const int index = (SIZE - 1) * SIZE + i;
                    cells.emplace_back(selected_neighbour.get_cell(index));
                    neighbour_cells.emplace_back(id,cells);
                }
                break;
            }
        case 2:
            {
                Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                const int index = (SIZE * SIZE) - SIZE;
                cells.emplace_back(selected_neighbour.get_cell(index));
                neighbour_cells.emplace_back(id,cells);
                break;
            }
        case 3:
            {
                Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                for (int i = 1; i < SIZE; i++)
                {
                    const int index = i * SIZE + (SIZE - 1);
                    cells.emplace_back(selected_neighbour.get_cell(index));
                    neighbour_cells.emplace_back(id,cells);
                }
                break;
            }
        case 4:
            {
                Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                for (int i = 1; i < SIZE; i++)
                {
                    const int index = i * SIZE;
                    cells.emplace_back(selected_neighbour.get_cell(index));
                    neighbour_cells.emplace_back(id,cells);
                }
                break;
            }
        case 5:
            {
                Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                const int index = SIZE - 1; // working maths backward for indexing
                cells.emplace_back(selected_neighbour.get_cell(index));
                neighbour_cells.emplace_back(id,cells);
                break;
            }
        case 6:
            {
                Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                for (int i = 1; i < SIZE; i++)
                {
                    int index = SIZE + i;
                    cells.emplace_back(selected_neighbour.get_cell(index));
                    neighbour_cells.emplace_back(id,cells);
                }
                break;
            }
        case 7:
            {
                Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                constexpr int index = 0; // working maths backward for indexing
                cells.emplace_back(selected_neighbour.get_cell(index));
                neighbour_cells.emplace_back(id,cells);
                break;
            }
        default: // cannot do anything if no key or id happen
            break;
        }
    }
}

void Manager::chunk_update(Chunk& buffer, Chunk& selected)
{
    const int size = selected.get_size();
    // Update only the inner region (size)
    for (int y = 1; y <= size; ++y) {
        for (int x = 1; x <= size; ++x) {
            // cell data
            int live = buffer.neighbour_count(x, y);
            bool current_cell = buffer.get_cell(x, y).is_alive();
            Cell& target = buffer.get_cell(x, y);

            // rule logic
            if (current_cell && (live < 2 || live > 3)) {
                target.set_type('0');
            } else if (!current_cell && live == 3) {
                target.set_type('w');
            } else {
                // preserve current state
                target.set_type(buffer.get_cell(x, y).get_type());
            }
        }
    }
    // After update, copy buffer’s inner region back into `selected`
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            selected.get_cell(x, y).set_type(buffer.get_cell(x+1, y+1).get_type());
        }
    }
}

void Manager::fill_halo_region(Chunk& buffer, const std::vector<std::pair<int, std::vector<Cell>>>& cells, const int size)
{
    if (cells.empty())
        return;

    for (auto [id, cellVec] : cells)
    {
        switch (id)
        {
        case 0: // North (top row)
            {
                for (int x = 0; x < size; ++x)
                    buffer.get_cell(x+1, 0).set_type(cellVec[x].get_type());
                break;
            }
        case 1: // North-East (top-right corner)
            {
                buffer.get_cell(size+1, 0).set_type(cellVec[0].get_type());
                break;
            }
        case 2: // East (right column)
            {
                for (int y = 0; y < size; ++y)
                    buffer.get_cell(size+1, y+1).set_type(cellVec[y].get_type());
                break;
            }
        case 3: // South-East (bottom-right corner)
            {
                buffer.get_cell(size+1, size+1).set_type(cellVec[0].get_type());
                break;
            }
        case 4: // South (bottom row)
            {
                for (int x = 0; x < size; ++x)
                    buffer.get_cell(x+1, size+1).set_type(cellVec[x].get_type());
                break;
            }
        case 5: // South-West (bottom-left corner)
            {
                buffer.get_cell(0, size+1).set_type(cellVec[0].get_type());
                break;
            }
        case 6: // West (left column)
            {
                for (int y = 0; y < size; ++y)
                    buffer.get_cell(0, y+1).set_type(cellVec[y].get_type());
                break;
            }
        case 7: // North-West (top-left corner)
            {
                buffer.get_cell(0, 0).set_type(cellVec[0].get_type());
                break;
            }
        default:
            break;
        }
    }
}

void Manager::construct_halo(Chunk& buffer, const Chunk& selected, const std::vector<std::pair<int, std::vector<Cell>>>& cells)
{
    const int size = selected.get_size() + CHUNK_OFF_SET;
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            buffer.get_cell(x + CHUNK_OFF_SET, y + CHUNK_OFF_SET).set_type(selected.get_cell(x, y).get_type());
        }
    }
    // fill in the halo of chunks
    fill_halo_region(buffer, cells, size);
}

void Manager::update()
{
    auto& world_data = world.get_world();
    for (auto&[key, chunk] : world_data)
    {
        int size = chunk.get_size();
        // neighbour data
        std::vector<long long> keys = world.get_neighbour_key(key);
        find_active_neighbour(keys, world_data); // looks for active chunks

        // cache buffer for writing to chunk
        Chunk buffer(chunk.get_CX(), chunk.get_CY(), chunk.get_size() + CHUNK_OFF_SET);

        // handles edge cases
        neighbours_cells_edge(world_data, size);
        construct_halo(buffer, chunk, neighbour_cells);
        chunk_update(buffer, chunk);
    }
}

void Manager::step()
{

}

void Manager::render_step()
{

}









