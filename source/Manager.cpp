#include "Manager.h"

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

std::vector<CellQuery> Manager::get_neighbour_queries()
{



};

void Manager::update()
{
    auto& selected_world = world.get_world();
    for (auto& [key, selected] : selected_world) {

        // finding out if key is alive in world
        std::vector<long long> list_neighbour_key = world.get_neighbour_key(key); // all keys for surrounding chunks
        std::vector<std::pair<int, long long>> active_neighbour; // finding out chunks that active
        for (long long neighbour_key : list_neighbour_key)
        {
            /* Chunks lay out:
             * 1 2 3    * is chunk that being updated
             * 4 * 5    - need to find out chunks that are
             * 6 7 8    - alive within world
             */
            int id = 1;
            if (selected_world.contains(neighbour_key))
            {
                active_neighbour.emplace_back(id, neighbour_key);
            }
            id++;
        }

        for (auto[id, neighbour_key] : active_neighbour)
        {
            if (id == 1 || id == 2 || id == 3)
            {
                // get bottom row of the chunk
                if (id == 1)
                    return;

                if (id == 2)
                    return;

                if (id == 3)
                    return;
            }

            if (id == 4 || id == 5)
            {

            }

            if (id == 6 || id == 7 || id == 8)
            {

            }
        }
        // Create temp buffer for next state
        Chunk buffer(selected.get_CX(), selected.get_CY(), selected.get_size());
        for (int i = 0; i < selected.get_size(); i++) {
            for (int j = 0; j < selected.get_size(); j++) {
                // selecting cell logic and adding changing buffer
                int live = world.neighbour_count(i, j); // live cells around i & j
                bool current_cell = selected.get_cell(i, j).is_alive(); // if it alive or not

                if (current_cell && (live < 2 || live > 3 ))
                {
                    buffer.get_cell(i, j).set_type('0');
                }
                else if (!current_cell && live == 3)
                {
                    buffer.get_cell(i, j).set_type('w');
                }
                else
                {
                    buffer.get_cell(i, j).set_type(
                        world.get_cell(i,j).get_type()
                        );
                }
            }
        }
        // load buffer back into selected chunk
        selected = std::move(buffer);
    }
}

Manager::~Manager() {};