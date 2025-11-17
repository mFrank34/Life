#include "Cache.h"
#include "CacheChunk.h"
#include <algorithm>
#include <cassert>

Cache::Cache() : World("Cache Map") {}

void Cache::unload()
{
    std::erase_if(chunks, [](auto &pair)
    {
        return !pair.second->is_populated();
    });
}

Chunk &Cache::get_chunk(int gx, int gy)
{
    int cx = floor_div(gx, CacheChunk::get_SIZE());
    int cy = floor_div(gy, CacheChunk::get_SIZE());
    long long key = generate_key(cx, cy);

    auto it = chunks.find(key);
    if (it != chunks.end())
        return *(it->second);   // dereference unique_ptr -> Chunk&

    // Create new CacheChunk and store as unique_ptr<Chunk>
    chunks[key] =

    return *chunks[key];
}

Cell &Cache::get_cell(int gx, int gy)
{
    Chunk &chunk = get_chunk(gx, gy);
    // safe runtime check:
    auto *cc = dynamic_cast<CacheChunk*>(&chunk);
    assert(cc && "Expected chunk to be CacheChunk");
    int lx = cc->get_LX(gx);
    int ly = cc->get_LY(gy);
    return cc->get_cell(lx, ly);
}

std::unordered_map<long long, std::unique_ptr<Chunk>> *Cache::get_world()
{
    return &chunks;
}
