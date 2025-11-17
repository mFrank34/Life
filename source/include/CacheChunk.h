#pragma once
#ifndef CACHECHUNK_H
#define CACHECHUNK_H

#include <unordered_map>
#include "World.h"
#include "DYNChunk.h"
#include "Cell.h"

class CacheChunk final: public DYNChunk<16>
{
public:
    CacheChunk(int ccx, int ccy);

    // cache logic for chunk
    int get_block_index();
    int get_inner_index();
}; 

#endif

