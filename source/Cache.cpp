#include "Cache.h"
#include "World.h"
#include <utility>
#include "Chunk.h"

// Constructor
Cache::Cache(const int size, const int max)
    : World("Cache Map"), CHUNK_SIZE(size), max_active(max) {}

// Remove empty chunks
void Cache::unload()
{
    std::erase_if(chunks, [](auto &pair)
                  { return !pair.second.is_populated(); });
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
    int cx = floor_div(gx,CHUNK_SIZE);
    int cy = floor_div(gy,CHUNK_SIZE);
    long long key = generate_key(gx, gy);
    return chunks.try_emplace(key, cx, cy, CHUNK_SIZE).first->second;
}

Chunk& Cache::get_chunk(const long long key)
{
    if (const auto it = chunks.find(key); it != chunks.end())
        return it->second;

    auto [cx, cy] = decode_key(key);
    return chunks.try_emplace(key, cx, cy, CHUNK_SIZE).first->second;
}

std::unordered_map<long long, Chunk>& Cache::get_world()
{
    return chunks;
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
    if (static_cast<int>(active.size()) > max_active) {
        long long evict_key = cached_keys.front();
        cached_keys.pop_front();
        active.erase(evict_key);
    }
    return chunk;
}
