#include "Manager.h"
#include "Unordered.h"
#include "Life.h"
#include "Chunk.h"

// includes
#include <unordered_map>

/*
Notes:
    Create a map manger for different map types. 
    - Unordered map 
    - Cache aware grids
    - Spares aware grids

    this need to manage all these different map system.
*/

Manager::Manager(Unordered *world, Life *rules)
{
    this->map = world;
    this->rules = rules;
};

void Manager::update()
{
    if (!map)
        return;

    map->unload(); // unload chunks

    // look for active chunks
    const std::unordered_map<long long, Chunk> chunks = map->get_world();
    for (const auto &selected : chunks)
    {
        Chunk chunk = selected.second;
        if (chunk.is_populated())
        {
            // wanna update the map so like...
        
        }
    }
};

Manager::~Manager() {};