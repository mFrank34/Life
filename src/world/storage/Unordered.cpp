/*
* File: Unordered.cpp
 * Author: Michael Franks
 * Description:
 */

#include "Unordered.h"

#include <mutex>
#include <unordered_map>
#include <vector>

Unordered::Unordered()
    : World("Unordered Map", 1)
{
}

void Unordered::unload()
{
    std::unique_lock lock(world_mtx);
    std::erase_if(world, [](auto& pair)
    {
        return !pair.second.is_populated();
    });
}

void Unordered::clear_world()
{
    std::unique_lock lock(world_mtx);
    world.clear();
}

Cell& Unordered::get_cell(const int global_x, const int global_y)
{
    Chunk& chunk = get_chunk(global_x, global_y);

    std::shared_lock lock(world_mtx);
    return chunk.get_cell(
        chunk.get_LX(global_x),
        chunk.get_LY(global_y)
    );
}

Chunk& Unordered::get_chunk(const int gx, const int gy)
{
    const int cx = floor_div(gx, CHUNK_SIZE);
    const int cy = floor_div(gy, CHUNK_SIZE);
    return get_chunk(generate_key(cx, cy));
}

Chunk& Unordered::get_chunk(const long long key)
{
    // Fast path — shared lock for existing chunk
    {
        std::shared_lock lock(world_mtx);
        auto it = world.find(key);
        if (it != world.end())
            return it->second;
    }

    // Slow path — exclusive lock for insertion
    std::unique_lock lock(world_mtx);

    // Check again in case another thread inserted while we waited
    auto it = world.find(key);
    if (it != world.end())
        return it->second;

    auto [cx, cy] = decode_key(key);
    auto [w_it, _] = world.try_emplace(key, cx, cy, CHUNK_SIZE);
    // Don't pre-create in step — let the update system handle it

    return w_it->second;
}
