/*
 * File: Unordered.cpp
 * Author: Michael Franks
 * Created: 2026-02-17
 * Last Edited: 2026-02-10
 * Description:
 */

#include "Unordered.h"
#include <unordered_map>
#include <vector>

Unordered::Unordered(const int size)
    : World("Unordered Map"), CHUNK_SIZE(size)
{
}

void Unordered::unload()
{
    std::erase_if(world, [](auto& pair)
    {
        return !pair.second.is_populated();
    });
}

Cell& Unordered::get_cell(const int global_x, const int global_y)
{
    Chunk& chunk = get_chunk(global_x, global_y);
    // Convert to local coordinates
    const int local_x = chunk.get_LX(global_x);
    const int local_y = chunk.get_LY(global_y);
    // hand over cell in vector
    return chunk.get_cell(local_x, local_y);
}

Chunk& Unordered::get_chunk(const int gx, const int gy)
{
    const int cx = floor_div(gx, CHUNK_SIZE);
    const int cy = floor_div(gy, CHUNK_SIZE);
    const long long key = generate_key(cx, cy);
    return get_chunk(key);
}

Chunk& Unordered::get_chunk(const long long key)
{
    // Fast path: already exists
    if (auto it = world.find(key); it != world.end())
        return it->second;

    // Decode chunk coordinates
    auto [cx, cy] = decode_key(key);

    // Create in current world
    auto [it, inserted] =
        world.try_emplace(key, cx, cy, CHUNK_SIZE);

    // Mirror creation in next world
    nextWorld.try_emplace(key, cx, cy, CHUNK_SIZE);

    return it->second;
}

std::unordered_map<long long, Chunk>& Unordered::get_world()
{
    return world; // std::unordered_map<long long, Chunk>
}

std::unordered_map<long long, Chunk>& Unordered::get_next_world()
{
    return nextWorld;
}

void Unordered::swap_world()
{
    world.swap(nextWorld);
}