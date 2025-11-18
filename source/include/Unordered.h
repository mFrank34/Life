#pragma once
#ifndef UNORDERED_H
#define UNORDERED_H

#include <unordered_map>
#include <vector>

// custom classes
#include "World.h"
#include "Chunk.h"
#include "Cell.h"

class Unordered : public World
{
public:
    Unordered(int size);

    // remove all empty chunks from world data
    void unload() override;

    // world entities
    Cell &get_cell(int global_x, int global_y) override;
    Chunk &get_chunk(int global_x, int global_y) override;

    // returning world data
    std::unordered_map<long long, Chunk> *get_world() override;

private:
    // system setup
    const int CHUNK_SIZE;

    // world data
    std::unordered_map<long long, Chunk> chunks;
};

#endif