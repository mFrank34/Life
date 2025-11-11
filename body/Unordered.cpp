#include "Unordered.h"
#include <unordered_map>
#include <vector>

Unordered::Unordered() : World("Unordered Map")
{
    
}

void Unordered::unload()
{
    // get the map data
    std::unordered_map<long long, Chunk>::iterator selected = chunks.begin();
    while (selected != chunks.end())
    {
        // gets chunk
        Chunk &chunk = selected->second;

        // finds if data is stored with in
        if (!chunk.is_populated()) // if not populated
        {
            selected = chunks.erase(selected);
        }
        else
        {
            ++selected;
        }
    }
}

Cell &Unordered::get_cell(int global_x, int global_y)
{
    int chunk_x = global_x / Chunk::CHUNK_SIZE;
    int chunk_y = global_y / Chunk::CHUNK_SIZE;

    // generating chunk
    Chunk &chunk = generate_chunk(chunk_x, chunk_y);

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
    // return the chunk
    return generate_chunk(chunk_x, chunk_y);
}

void* Unordered::get_world()
{
    // i agree this looks bad!
    return &chunks; // std::unordered_map<long long, Chunk>
}

Chunk& Unordered::generate_chunk(int chunk_x, int chunk_y)
{
    long long key = generate_key(chunk_x, chunk_y);
    // if the chunk doesn't exist, create new chunk
    if (chunks.find(key) == chunks.end())
    {
        chunks.emplace(key, Chunk(chunk_x, chunk_y));
    };
    // creates chunk
    return chunks.at(key);
}