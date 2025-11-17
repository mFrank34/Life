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
    Sparse();

    void unload() override;

    Cell &get_cell(int gx, int gy) override;
    Chunk &get_chunk(int gx, int gy) override;

    std::unordered_map<long long, Chunk> *get_world() override;

private:
    std::unordered_map<long long, Chunk> chunks{};
};

#endif