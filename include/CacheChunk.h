#pragma once
#ifndef CACHECHUNK_H
#define CACHECHUNK_H

#include <unordered_map>
#include "World.h"
#include "Chunk.h"
#include "Cell.h"

class CacheChunk : public Chunk
{
public:
    CacheChunk(int ccx, int ccy);

    // cache logic for chunk
    int get_block_index();
    int get_inner_index();

private:
    int cx = 0, cy = 0;
    static const int SIZE = 16;
    Cell cells[SIZE][SIZE];

}; 

#endif

