#include "Unordered.h"
#include <unordered_map>
#include <vector>

Unordered::Unordered(const int size)
    : World("Unordered Map"), CHUNK_SIZE(size)
{

}

void Unordered::unload()
{
    std::erase_if(chunks, [](auto &pair)
                  { return !pair.second.is_populated(); });
}

Cell &Unordered::get_cell(int global_x, int global_y)
{
    Chunk& chunk = get_chunk(global_x, global_y);
    // Convert to local coordinates
    int local_x = chunk.get_LX(global_x);
    int local_y = chunk.get_LY(global_y);
    // hand over cell in vector
    return chunk.get_cell(local_x, local_y);
}

Chunk &Unordered::get_chunk(int global_x, int global_y)
{
    // reversed the cell function to return chunk as well
    int cx = floor_div(global_x, CHUNK_SIZE);
    int cy = floor_div(global_y, CHUNK_SIZE);
    // key
    long long key = generate_key(cx, cy);
    auto [it, inserted] = chunks.try_emplace(key, cx, cy, CHUNK_SIZE);
    return it->second;
}

std::unordered_map<long long, Chunk> *Unordered::get_world()
{
    return &chunks; // std::unordered_map<long long, Chunk>
}