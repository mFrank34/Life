#pragma once
#ifndef CACHECHUNK_H
#define CACHECHUNK_H

#include "World.h"
#include "Chunk.h"
#include "Cell.h"

struct CacheBlock
{
    static constexpr int BLOCK_SIZE = 4;
    Cell cells[BLOCK_SIZE][BLOCK_SIZE];

    // Access a cell inside the block
    Cell &get(int lx, int ly);
};

class CacheChunk : public Chunk
{
public:
    CacheChunk(int cx_, int cy_);

    int get_local_x(int global_x) const;
    int get_local_y(int global_y) const;

    int get_block_index(int local) const;
    int get_inner_index(int local) const;

    CacheBlock &get_block(int block_x, int block_y);
    Cell &get_cell(int global_x, int global_y);

    bool is_populated() const;

private:
    std::unordered_map<long long, CacheBlock> blocks;
};

#endif