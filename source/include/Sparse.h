#pragma once
#ifndef SPARSE_H
#define SPARSE_H

#include <unordered_map>
// custom classes
#include "World.h"
#include "Chunk.h"
#include "Cell.h"

class Sparse final : public World
{
public:
    Sparse();
    // unload inactive chunks with no alive cells
    void unload() override;
    // cells & chunks
    Cell &get_cell(int gx, int gy) override;
    Chunk &get_chunk(int gx, int gy) override;
    // get world
    std::unordered_map<long long, std::unique_ptr<Chunk>> *get_world() override;

private:
    std::unordered_map<long long, std::unique_ptr<Chunk>> chunks{};
};

#endif