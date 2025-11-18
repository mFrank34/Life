#pragma once
#ifndef CACHECHUNK_H
#define CACHECHUNK_H

#include "BaseChunk.h"

class CacheChunk final : public BaseChunk
{
public:
    CacheChunk(int ccx, int ccy);
    // cell instances
    Cell& get_cell(int x, int y) override;
    const Cell& get_cell(int x, int y) const override;
    // view chunk
    void print_chunk() const override;
    // stats of chunks
    bool is_populated() const override;
    int populate_amt() const override;

private:
    static constexpr int size = 16;
    Cell cells [size][size];
}; 

#endif
