#include "Unordered.h"
#include "Chunk.h"
#include <unordered_map>
#include <algorithm>
#include <memory>


Unordered::Unordered() : World("Unordered Map") {}

void Unordered::unload()
{
    std::erase_if(chunks, [](auto &pair)
                  { return !pair.second->is_populated(); });
}

Cell &Unordered::get_cell(int global_x, int global_y)
{
    Chunk &chunk = get_chunk(global_x, global_y);

    int local_x = chunk.get_LX(global_x);
    int local_y = chunk.get_LY(global_y);

    return chunk.get_cell(local_x, local_y);
}

Chunk &Unordered::get_chunk(int global_x, int global_y)
{
    int cx = floor_div(global_x, Chunk::get_SIZE());
    int cy = floor_div(global_y, Chunk::get_SIZE());
    long long key = generate_key(cx, cy);

    // Return existing chunk if found
    auto it = chunks.find(key);
    if (it != chunks.end())
        return *(it->second);

    chunks[key] = std::make_unique<Chunk>(cx, cy);
    return *chunks[key];
}

std::unordered_map<long long, std::unique_ptr<Chunk>> *Unordered::get_world()
{
    return &chunks; // std::unordered_map<long long, Chunk>
}