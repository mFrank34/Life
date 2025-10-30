#pragma once
#ifndef MAP_H
#define MAP_H

#include <unordered_map>
#include "Chunk.h"
#include "Debug.h"

class Map
{
public:
    Map();

    // adding debugging
    Debug debug;

    // gets unordered map of chunks
    std::unordered_map<long long, Chunk> get_world();

    // gets a reference to a cell using global coordinates
    Cell &get_cell(int global_x, int global_y);

    // get chunk a reference to chunk using global coors
    Chunk &get_chunk(int global_x, int global_y);

    int neighbour_count(int global_x, int global_y);

    void unload();

private:
    // finds or generated a chunk
    Chunk &generate_chunk(int chunk_x, int chunk_y);

    // data structure for storing chunks
    std::unordered_map<long long, Chunk> chunks;

    // Packs (chunk_x, chunk_y) into single 64-bit key
    long long generate_key(int chunk_x, int chunk_y) const;
};

#endif