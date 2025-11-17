#include "Sparse.h"
#include "Chunk.h"
#include <algorithm>
#include <memory>


Sparse::Sparse() : World("Sparse Map") {}

void Sparse::unload()
{
    std::erase_if(chunks, [](auto &pair)
                  { return !pair.second->is_populated(); });
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
    int cx = floor_div(gx, Chunk::get_SIZE());
    int cy = floor_div(gy, Chunk::get_SIZE());
    long long key = generate_key(cx, cy);

    chunks[key] = std::make_unique<Chunk>(gx, gy);
    return *chunks[key];
}

std::unordered_map<long long, std::unique_ptr<Chunk>>* Sparse::get_world()
{
    return &chunks;
}
