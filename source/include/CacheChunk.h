#pragma once
#ifndef CACHECHUNK_H
#define CACHECHUNK_H

#include "DYNChunk.h"

class CacheChunk final: public DYNChunk<16>
{
public:
    CacheChunk(int ccx, int ccy);
}; 

#endif
