#include "Cache.h"
#include "Chunk.h"
#include ""

// Constructor
Cache::Cache(int size)
    : World("Cache Map"),
{

}

// Remove empty chunks
void Cache::unload()
{
    std::erase_if(chunks, [](auto& pair)
    {
        return !pair.second.is_populated();
    });
}

Cell& Cache::get_cell(int gx, int gy)
{
    Chunk& base = get_chunk(gx, gy);
    return base.get_cell(gx, gy);
}

Chunk& Cache::get_chunk(int gx, int gy)
{
    // floor_div ensures negative coordinates work correctly
    int cx = floor_div(gx, CacheChunk::get_SIZE());
    int cy = floor_div(gy, CacheChunk::get_SIZE());
    long long key = generate_key(cx, cy);

    // Emplace a new CacheChunk if it doesn't exist
    return chunks.try_emplace(key, CacheChunk{cx, cy}).first->second;
}


std::unordered_map<long long, Chunk>* Cache::get_world()
{
    // reinterpret_cast needed because chunks store CacheChunk
    return reinterpret_cast<std::unordered_map<long long, Chunk>*>(&chunks);
}
