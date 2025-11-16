#include "Unordered.h"
#include <unordered_map>
#include <vector>

Unordered::Unordered() : World("Unordered Map") {}

void Unordered::unload()
{
    std::erase_if(chunks, [](auto &pair)
                  { return !pair.second.is_populated(); });
}

Cell &Unordered::get_cell(int global_x, int global_y)
{
    Chunk &chunk = get_chunk(global_x, global_y);
    int cx = floor_div(global_x, chunk.get_SIZE());
    int cy = floor_div(global_y, chunk.get_SIZE());
    long long key = generate_key(cx, cy);

    // generating chunk
    chunks.try_emplace(key, cx, cy).first->second;

    // cords maths for x and y
    int local_x = chunk.get_LX(global_x);
    int local_y = chunk.get_LY(global_y);

    // return the cell
    return chunk.get_cell(local_x, local_y);
}

Chunk &Unordered::get_chunk(int global_x, int global_y)
{
    // reversed the cell function to return chunk as well
    int cx = floor_div(global_x, Chunk::get_SIZE());
    int cy = floor_div(global_y, Chunk::get_SIZE());
    // key
    long long key = generate_key(cx, cy);
    return chunks.try_emplace(key, cx, cy).first->second;
}

std::unordered_map<long long, Chunk> *Unordered::get_world()
{
    return &chunks; // std::unordered_map<long long, Chunk>
}