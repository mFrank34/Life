#include "Sparse.h"
#include "Chunk.h"

Sparse::Sparse(const int size)
    : World("Sparse Map"), CHUNK_SIZE(size)
{

}

void Sparse::unload()
{
    std::erase_if(chunks, [](auto &pair)
                  { return !pair.second.is_populated(); });
}

Cell &Sparse::get_cell(int gx, int gy)
{
    Chunk &chunk = get_chunk(gx, gy);
    return chunk.get_cell(
        chunk.get_LX(gx),
        chunk.get_LY(gy)
        );
}

Chunk &Sparse::get_chunk(int gx, int gy)
{
    int cx = floor_div(gx, CHUNK_SIZE);
    int cy = floor_div(gy, CHUNK_SIZE);
    long long key = generate_key(cx, cy);

    return chunks.try_emplace(key, cx, cy, CHUNK_SIZE).first->second;
}

std::unordered_map<long long, Chunk> *Sparse::get_world()
{
    return &chunks;
}
