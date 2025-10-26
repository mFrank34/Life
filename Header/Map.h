#ifndef MAP_H
#define MAP_H

#include <unordered_map>
#include "Chunk.h"

class Map
{
public:
    // gets a reference to a cell using global coordinates
    Cell &get_cell(int global_x, int global_y);

    // get chunk a reference to chunk using global coors
    Chunk &get_chunk(int global_x, int global_y);

private:
    // finds or generated a chunk
    Chunk &generate_chunk(int chunk_x, int chunk_y);

    // data structure for storing chunks
    std::unordered_map<long long, Chunk> chunks;

    // Packs (chunk_x, chunk_y) into single 64-bit key
    long long generate_key(int chunk_x, int chunk_y) const;
};

#endif