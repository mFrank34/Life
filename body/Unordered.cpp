#include "Unordered.h"
#include <unordered_map>
#include <vector>

Unordered::Unordered() : World("Unordered Map")
{

}

void Unordered::unload()
{
    std::erase_if(chunks, [](auto &pair) 
    { 
        return !pair.second.is_populated(); 
    });
}

Cell &Unordered::get_cell(int global_x, int global_y)
{
    int chunk_x = global_x / Chunk::CHUNK_SIZE;
    int chunk_y = global_y / Chunk::CHUNK_SIZE;
    long long key = generate_key(chunk_x, chunk_y);
    // generating chunk
    Chunk &chunk = chunks.try_emplace(key, chunk_x, chunk_y).first->second;

    // cords maths for x and y
    int local_x = chunk.local_x(global_x);
    int local_y = chunk.local_y(global_y);

    // return the cell
    return chunk.get_cell(local_x, local_y);
}

Chunk &Unordered::get_chunk(int global_x, int global_y)
{
    // reversed the cell function to return chunk as well
    int chunk_x = global_x / Chunk::CHUNK_SIZE;
    int chunk_y = global_y / Chunk::CHUNK_SIZE;
    // key
    long long key = generate_key(chunk_x, chunk_y);
    return chunks.try_emplace(key, chunk_x, chunk_y).first->second;
}

std::unordered_map<long long, Chunk> *Unordered::get_world()
{
    // i agree this looks bad!
    return &chunks; // std::unordered_map<long long, Chunk>
}