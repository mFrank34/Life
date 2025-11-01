#include "Manager.h"
#include "Map.h"
#include "Life.h"
#include "Chunk.h"

// includes
#include <unordered_map>

Manager::Manager(Map *world)
{
    this->map = world;
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
            chunk.update(rules);
        }
    }
};

Manager::~Manager() {};