/*
 * File: Sparse.cpp
 * Author: Michael Franks
 * Description:
 */

#include "Sparse.h"
#include "world/structure/Chunk.h"
#include "world/World.h"

Sparse::Sparse(const int size)
    : World("Sparse Map", size)
{
}

void Sparse::unload()
{
    std::erase_if(world, [](auto& pair)
    {
        return !pair.second.is_populated();
    });
}

void Sparse::clear_world()
{
    world.clear();
}

Cell& Sparse::get_cell(const int gx, const int gy)
{
    Chunk& chunk = get_chunk(gx, gy);
    return chunk.get_cell(
        chunk.get_LX(gx),
        chunk.get_LY(gy)
    );
}

Chunk& Sparse::get_chunk(const int gx, const int gy)
{
    int cx = floor_div(gx, CHUNK_SIZE);
    int cy = floor_div(gy, CHUNK_SIZE);
    const long long key = generate_key(cx, cy);

    return get_chunk(key);
}


Chunk& Sparse::get_chunk(const long long key)
{
    auto it = world.find(key);
    if (it != world.end())
        return it->second;

    auto [cx, cy] = decode_key(key);

    // Create in BOTH maps
    auto [w_it, _] = world.try_emplace(
        key, cx, cy, CHUNK_SIZE
    );

    step.try_emplace(
        key, cx, cy, CHUNK_SIZE
    );

    return w_it->second;
}