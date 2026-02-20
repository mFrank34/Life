/*
 * File: World.cpp
 * Author: Michael Franks
 * Description: Base class container for world-class
 */

#include "World.h"
#include <array>


World::World(const std::string_view& TYPE, const int CHUNK_SIZE)
    : TYPE(TYPE), CHUNK_SIZE(CHUNK_SIZE)
{
}

std::unordered_map<long long, Chunk>& World::get_world()
{
    return world;
}

std::unordered_map<long long, Chunk>& World::get_stepped_world()
{
    return step;
}

void World::swap_world()
{
    world.swap(step);
    step.clear();
}

std::array<long long, 8> World::get_neighbour_key(long long key) const
{
    auto [x, y] = decode_key(key);
    return {
        generate_key(x, y - 1), // N
        generate_key(x + 1, y - 1), // NE
        generate_key(x + 1, y), // E
        generate_key(x + 1, y + 1), // SE
        generate_key(x, y + 1), // S
        generate_key(x - 1, y + 1), // SW
        generate_key(x - 1, y), // W
        generate_key(x - 1, y - 1) // NW
    };
}

std::string World::get_type() const
{
    return TYPE;
}

int World::get_size() const
{
    return CHUNK_SIZE;
}

void World::tick(float delta)
{
}

// protected
long long World::generate_key(const int chunk_x, int chunk_y)
{
    return (static_cast<long long>(chunk_x) << 32) | (static_cast<unsigned int>(chunk_y));
}

// protected
std::pair<int, int> World::decode_key(const long long key)
{
    auto chunk_x = static_cast<int>(key >> KEY_LENGTH); // decode key 32bit
    auto chunk_y = static_cast<int>(key & 0xFFFFFFFF); // -1 32bit
    return {chunk_x, chunk_y};
}

int World::floor_div(const int cord, const int size)
{
    int div = cord / size;
    if ((cord < 0) && (cord % size != 0))
        div--; // move to the correct negative chunk
    return div;
}
