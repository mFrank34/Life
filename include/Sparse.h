#pragma once
#ifndef SPARSE_H
#define SPARSE_H

#include <unordered_map>
#include <vector>

// custom classes
#include "World.h"
#include "Chunk.h"
#include "Cell.h"

class Sparse : public World
{
public:
    Sparse();

    // utility for safe chunk indexing
    static inline int chunk_index(int v, int size);

    // cell
    Cell &get_cell(int global_x, int global_y);
    Cell &set_cell(int global_x, int global_y);
    // chunk
    Chunk &get_chunk(int global_x, int global_y);
    Chunk &try_get_chunk(int global_x, int global_y);
    // remove empty chunks
    void unload();

    // pointer to world data
    std::unordered_map<long long, Chunk> *get_world() override;

private:
    std::unordered_map<long long, Chunk> chunks;
};

#endif