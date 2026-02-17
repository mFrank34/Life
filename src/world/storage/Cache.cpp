/*
 * File: Cache.cpp
 * Author: Michael Franks
 * Description:
 */

#include "Cache.h"
#include "world/World.h"
#include <utility>
#include "world/structure/Chunk.h"

// Constructor
Cache::Cache(const int size, const int max)
    : World("Cache Map"), CHUNK_SIZE(size), max_active(max)
{

}

// Remove empty chunks
void Cache::unload()
{
    std::erase_if(world, [](auto& pair)
    {
        return !pair.second.is_populated();
    });
    active.clear();
    cached_keys.clear();
}

Cell& Cache::get_cell(const int gx, const int gy)
{
    Chunk& chunk = get_cached_chunk(gx, gy);
    return chunk.get_cell(
        chunk.get_LX(gx),
        chunk.get_LY(gy)
    );
}

Chunk& Cache::get_chunk(const int gx, const int gy)
{
    int cx = floor_div(gx, CHUNK_SIZE);
    int cy = floor_div(gy, CHUNK_SIZE);
    const long long key = generate_key(cx, cy);

    return get_chunk(key);
}

Chunk& Cache::get_chunk(const long long key)
{
    auto it = world.find(key);
    if (it != world.end())
        return it->second;

    auto [cx, cy] = decode_key(key);

    // Create in BOTH maps
    auto [w_it, _] = world.try_emplace(
        key, cx, cy, CHUNK_SIZE
    );

    nextWorld.try_emplace(
        key, cx, cy, CHUNK_SIZE
    );

    return w_it->second;
}

std::unordered_map<long long, Chunk>& Cache::get_world()
{
    return world;
}

std::unordered_map<long long, Chunk>& Cache::get_next_world()
{
    return nextWorld;
}

void Cache::swap_world()
{
    world.swap(nextWorld);
}

Chunk& Cache::get_cached_chunk(const int gx, const int gy)
{
    Chunk& chunk = get_chunk(gx, gy);
    const long long key = generate_key(gx, gy);
    // if already active look though cached keys
    if (const auto it = active.find(key); it != active.end())
    {
        // Already cached: refresh LRU
        cached_keys.remove(key);
        cached_keys.push_back(key);
        return *it->second;
    }
    active[key] = &chunk;
    cached_keys.push_back(key);
    // Evict if over capacity
    if (static_cast<int>(active.size()) > max_active)
    {
        long long evict_key = cached_keys.front();
        cached_keys.pop_front();
        active.erase(evict_key);
    }
    return chunk;
}