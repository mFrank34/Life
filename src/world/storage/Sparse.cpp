/*
* File: Sparse.cpp
 * Author: Michael Franks
 * Description:
 */

#include "Sparse.h"

#include <mutex>

#include "world/structure/Chunk.h"
#include "world/World.h"

Sparse::Sparse(const int size)
    : World("Sparse Map", size)
{
}

void Sparse::unload()
{
    std::unique_lock lock(chunk_mtx);
    std::erase_if(world, [](auto& pair)
    {
        return !pair.second.is_populated();
    });
}

void Sparse::clear_world()
{
    std::unique_lock lock(chunk_mtx);
    world.clear();
}

Cell& Sparse::get_cell(const int gx, const int gy)
{
    Chunk& chunk = get_chunk(gx, gy);

    // Shared lock for reading cell — multiple threads can read simultaneously
    std::shared_lock lock(chunk_mtx);
    return chunk.get_cell(
        chunk.get_LX(gx),
        chunk.get_LY(gy)
    );
}

Chunk& Sparse::get_chunk(const int gx, const int gy)
{
    int cx = floor_div(gx, CHUNK_SIZE);
    int cy = floor_div(gy, CHUNK_SIZE);
    return get_chunk(generate_key(cx, cy));
}

Chunk& Sparse::get_chunk(const long long key)
{
    // Fast path — shared lock for existing chunk
    {
        std::shared_lock lock(chunk_mtx);
        auto it = world.find(key);
        if (it != world.end())
            return it->second;
    }

    // Slow path — exclusive lock for insertion
    std::unique_lock lock(chunk_mtx);

    // Check again in case another thread inserted while we waited
    auto it = world.find(key);
    if (it != world.end())
        return it->second;

    auto [cx, cy] = decode_key(key);
    auto [w_it, _] = world.try_emplace(key, cx, cy, CHUNK_SIZE);
    step.try_emplace(key, cx, cy, CHUNK_SIZE);

    return w_it->second;
}