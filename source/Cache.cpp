#include "Cache.h"
#include <utility>
#include "Chunk.h"


// Constructor
Cache::Cache(const int size, const int max)
    : World("Cache Map"), CHUNK_SIZE(size), max_activa(max) {}

// Remove empty chunks
void Cache::unload()
{
    std::erase_if(chunks, [](auto &pair)
                  { return !pair.second.is_populated(); });
    active.clear();
    cached_keys.clear();
}

Cell& Cache::get_cell(int gx, int gy)
{
    Chunk& chunk = get_cached_chunk(gx, gy);
    return chunk.get_cell(
        chunk.get_LX(gx),
        chunk.get_LY(gy)
        );
}

Chunk& Cache::get_chunk(int gx, int gy)
{
    int cx = floor_div(gx,CHUNK_SIZE);
    int cy = floor_div(gy,CHUNK_SIZE);
    long long key = generate_key(cx, cy);
    return chunks.try_emplace(key, Chunk(gx, gy, CHUNK_SIZE)).first->second;
}

std::unordered_map<long long, Chunk>* Cache::get_world()
{
    return &chunks;
}

Chunk& Cache::get_cached_chunk(int gx, int gy)
{
    Chunk& chunk = get_chunk(gx, gy);
    long long key = generate_key(gx, gy);

    // if already active look though cached keys
    auto it = active.find(key);
    if (it == active.end())
    {
        cached_keys.remove(key);
        cached_keys.push_back(key);
        return *it->second;
    }

    // add new chunk to cached
    active[key] = &chunk;
    cached_keys.push_front(key);

    // evict if over capacity
    if ((int)active.size() > max_activa)
    {
        long long evict_key = cached_keys.back();
        cached_keys.pop_back();
        active.erase(evict_key);
    }

    return chunk;
}
