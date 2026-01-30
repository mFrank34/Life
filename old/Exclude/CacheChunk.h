#pragma once
#ifndef CACHECHUNK_H
#define CACHECHUNK_H

#include <vector>
#include "../../source/include/BaseChunk.h"

class CacheChunk final : public BaseChunk
{
public:

    CacheChunk(int ccx, int ccy, int ccs);
    // cell instances
    Cell& get_cell(int x, int y) override;
    const Cell& get_cell(int x, int y) const override;
    // view chunk
    void print_chunk() const override;
    // stats of chunks
    bool is_populated() const override;
    int populated_amt() const override;

private:
    int size;
    std::vector<Cell> cells;
}; 

#endif
