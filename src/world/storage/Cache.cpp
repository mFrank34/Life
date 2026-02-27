/*
 * File: Cache.cpp
 * Author: Michael Franks
 * Description: Cache Storage Container
 */

#include "Cache.h"

#include <mutex>

#include "world/World.h"
#include <utility>
#include "world/structure/Chunk.h"

Cache::Cache(const int size, const int max)
    : World("Cache Map", size), max_active(max)
{
}

void Cache::unload()
{
    std::unique_lock lock(world_mtx);
    std::lock_guard cache_lock(cache_mtx);

    std::erase_if(world, [](auto& pair)
    {
        return !pair.second.is_populated();
    });

    active.clear();
    cached_keys.clear();
}

void Cache::clear_world()
{
    std::unique_lock lock(world_mtx);
    std::lock_guard cache_lock(cache_mtx);

    world.clear();
    active.clear();
    cached_keys.clear();
}

Cell& Cache::get_cell(const int gx, const int gy)
{
    Chunk& chunk = get_cached_chunk(gx, gy);

    std::shared_lock lock(world_mtx);
    return chunk.get_cell(
        chunk.get_LX(gx),
        chunk.get_LY(gy)
    );
}

Chunk& Cache::get_chunk(const int gx, const int gy)
{
    int cx = floor_div(gx, CHUNK_SIZE);
    int cy = floor_div(gy, CHUNK_SIZE);
    return get_chunk(generate_key(cx, cy));
}

Chunk& Cache::get_chunk(const long long key)
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

Chunk& Cache::get_cached_chunk(const int gx, const int gy)
{
    int cx = floor_div(gx, CHUNK_SIZE);
    int cy = floor_div(gy, CHUNK_SIZE);
    const long long key = generate_key(cx, cy);

    // Check LRU cache first
    {
        std::lock_guard cache_lock(cache_mtx);
        if (const auto it = active.find(key); it != active.end())
        {
            // Already cached — refresh LRU
            cached_keys.remove(key);
            cached_keys.push_back(key);
            return *it->second;
        }
    }

    // Not cached — get or create chunk
    Chunk& chunk = get_chunk(key);

    // Insert into LRU cache
    {
        std::lock_guard cache_lock(cache_mtx);

        active[key] = &chunk;
        cached_keys.push_back(key);

        // Evict oldest if over capacity
        if (static_cast<int>(active.size()) > max_active)
        {
            long long evict_key = cached_keys.front();
            cached_keys.pop_front();
            active.erase(evict_key);
        }
    }

    return chunk;
}
