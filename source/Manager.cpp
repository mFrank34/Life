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

};

void Manager::update()
{
    auto& selected_world = world.get_world();
    for (auto& [key, selected] : selected_world) {

        std::vector<long long> neighbour_key = world.get_neighbour_key(key);



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