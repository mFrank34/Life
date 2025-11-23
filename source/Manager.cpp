#include "Manager.h"

#include <gtk-4.0/gtk/gtkgestureswipe.h>

#include "Rules.h"
#include "World.h"
#include "Chunk.h"

/*
Notes:
    Create a map manger for different map types.
    - Unordered map
    - Cache aware grids
    - Spares aware grids

    this need to manage all these different map system.
*/

Manager::Manager(World& world, Rules& rules)
    : world(world), rules(rules)
{

}

void Manager::update()
{
    auto& selected_world = world.get_world();
    for (auto& [key, selected] : selected_world) {
        int selected_chunk_size = selected.get_size();
        // finding out if key is alive in world | all keys for surrounding chunks
        std::vector<long long> list_neighbour_key = world.get_neighbour_key(key);

        // Create temp buffer for next state | off-setting the chunk to make sightly larger chunk
        Chunk buffer(selected.get_CX(), selected.get_CY(), (selected.get_size() + CHUNK_OFF_SET));


        if (!active_neighbour.empty())
            Neighbours_cells_edges(selected_world);

        // interacting with the current selected chunk
        for (int i = 0; i < selected.get_size(); i++) {
            for (int j = 0; j < selected.get_size(); j++) {
                // selecting cell logic and adding changing buffer
                int live = world.neighbour_count(i, j);
                // live cells around i & j
                bool current_cell = selected.get_cell(i, j).is_alive(); // if it alive or not
                // these need to shifted over by CHUNK_OFF_SET to fit in buffer chunk
                if (current_cell && (live < 2 || live > 3 ))
                {
                    buffer.get_cell(i + CHUNK_OFF_SET, j + CHUNK_OFF_SET).set_type('0');
                }
                else if (!current_cell && live == 3)
                {
                    buffer.get_cell(i + CHUNK_OFF_SET , j + CHUNK_OFF_SET).set_type('w');
                }
                else
                {
                    buffer.get_cell(i + CHUNK_OFF_SET, j + CHUNK_OFF_SET).set_type(
                        world.get_cell(i,j).get_type()
                        );
                }
            }
        }
        // load buffer back into selected chunk
        selected = std::move(buffer);
    }
}

void Manager::find_active_neighbour(auto& keys, auto& selected)
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
        id++;
    }
}

void Manager::Neighbours_cells_edges(auto& selected_world)
{
    int selected_chunk_size = selected_world.get_chunk_size();
    for (auto[id, neighbour_key] : active_neighbour)
        {
            std::vector<Cell> cells;
            switch (id) // id is number around chunks
            {
            case 0:
                {   // 1 cell every time
                    Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                    const int index = (selected_chunk_size * selected_chunk_size) - 1; // working maths backward for indexing
                    cells.emplace_back(selected_neighbour.get_cell(index));
                    neighbour_cells.emplace_back(id,cells);
                    break;
                }
            case 1:
                {   // multi-able cells depending on size
                    Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                    for (int i = 1; i < selected_chunk_size; i++)
                    {
                        const int index = (selected_chunk_size - 1) * selected_chunk_size + i;
                        cells.emplace_back(selected_neighbour.get_cell(index));
                        neighbour_cells.emplace_back(id,cells);
                    }
                    break;
                }
            case 2:
                {
                    Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                    const int index = (selected_chunk_size * selected_chunk_size) - selected_chunk_size;
                    cells.emplace_back(selected_neighbour.get_cell(index));
                    neighbour_cells.emplace_back(id,cells);
                    break;
                }
            case 3:
                {
                    Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                    for (int i = 1; i < selected_chunk_size; i++)
                    {
                        const int index = i * selected_chunk_size + (selected_chunk_size - 1);
                        cells.emplace_back(selected_neighbour.get_cell(index));
                        neighbour_cells.emplace_back(id,cells);
                    }
                    break;
                }
            case 4:
                {
                    Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                    for (int i = 1; i < selected_chunk_size; i++)
                    {
                        const int index = i * selected_chunk_size;
                        cells.emplace_back(selected_neighbour.get_cell(index));
                        neighbour_cells.emplace_back(id,cells);
                    }
                    break;
                }
            case 5:
                {
                    Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                    const int index = selected_chunk_size - 1; // working maths backward for indexing
                    cells.emplace_back(selected_neighbour.get_cell(index));
                    neighbour_cells.emplace_back(id,cells);
                    break;
                }
            case 6:
                {
                    Chunk selected_neighbour = selected_world.find(neighbour_key)->second;
                    for (int i = 1; i < selected_chunk_size; i++)
                    {
                        int index = selected_chunk_size + i;
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
