#pragma once
#ifndef SPARSE_H
#define SPARSE_H

#include <unordered_map>
#include <vector>

// custom classes
#include "World.h"
#include "Chunk.h"
#include "Cell.h"

class Sparse final : public World
{
public:
    Sparse(int size);
    // unloads chunks in memory by removing them.
    void unload() override;
    // get world entities
    Cell &get_cell(int gx, int gy) override;
    Chunk &get_chunk(int gx, int gy) override;
    Chunk &get_chunk(long long key) override;
    void swap_world() override;

    // returning world data
    std::unordered_map<long long, Chunk>& get_world() override;
    std::unordered_map<long long, Chunk>& get_next_world() override;

private:
    const int CHUNK_SIZE;
    std::unordered_map<long long, Chunk> world;

    std::unordered_map<long long, Chunk> nextWorld;
};

#endif