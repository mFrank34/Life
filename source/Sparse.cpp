#include "Sparse.h"

Sparse::Sparse() : World("Sparse Map") {}

void Sparse::unload()
{
    std::erase_if(chunks, [](auto &pair)
                  { return !pair.second.is_populated(); });
}

Cell &Sparse::get_cell(int global_x, int global_y)
{
    Chunk &chunk = get_chunk(global_x, global_y);
    return chunk.get_cell(
        chunk.get_LX(global_x),
        chunk.get_LX(global_y));
}

Chunk &Sparse::get_chunk(int global_x, int global_y)
{
    int cx = floor_div(global_x, Chunk::get_SIZE());
    int cy = floor_div(global_y, Chunk::get_SIZE());
    long long key = generate_key(cx, cy);

    return chunks.try_emplace(key, cx, cy).first->second;
}

std::unordered_map<long long, Chunk> *Sparse::get_world()
{
    return &chunks;
}
