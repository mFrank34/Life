#include "Map.h"

long long Map::generate_key(int chunk_x, int chunk_y) const
{
    // packs two 32 bit ints into one 64-bit key for the map
    return (static_cast<long long>(chunk_x) << 32) | (static_cast<unsigned int>(chunk_y));
}

Chunk &Map::generate_chunk(int chunk_x, int chunk_y)
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

Cell& Map::get_cell(int global_x, int global_y)
{
    int chunk_x = global_x / Chunk::CHUNK_SIZE;
    int chunk_y = global_y / Chunk::CHUNK_SIZE;

    // // ensure negative coordinates but we don't care to much about
    // if (global_x < 0) chunk_x--;
    // if (global_y < 0) chunk_y--;

    // generating chunk
    Chunk& chunk = generate_chunk(chunk_x, chunk_y);

    // cords maths for x and y 
    int local_x = global_x - chunk_x * Chunk::CHUNK_SIZE;
    int local_y = global_y - chunk_y * Chunk::CHUNK_SIZE;

    // return the cell
    return chunk.get_cell(local_x, local_y);
}

Chunk& Map::get_chunk(int global_x, int global_y)
{
    // reversed the cell function to return chunk as well
    int chunk_x = global_x / Chunk::CHUNK_SIZE;
    int chunk_y = global_y / Chunk::CHUNK_SIZE;

    // return the chunk
    return generate_chunk(chunk_x, chunk_y);
}
