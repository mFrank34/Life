#pragma once
#ifndef UNORDERED_H
#define UNORDERED_H

#include <unordered_map>
#include <vector>

// custom classes
#include "World.h"
#include "DYNChunk.h"
#include "Cell.h"

class Unordered : public World
{
public:
    Unordered();

    // remove all empty chunks from world data
    void unload() override;

    // world entities
    Cell &get_cell(int global_x, int global_y) override;
    Chunk &get_chunk(int global_x, int global_y) override;

    // world data
    std::unordered_map<long long, Chunk> *get_world() override;

private:
    std::unordered_map<long long, Chunk> chunks;
};

#endif