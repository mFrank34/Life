#pragma once
#ifndef CACHE_H
#define CACHE_H

#include "World.h"
#include "Cell.h"
#include "Chunk.h"

class Cache final : public World
{
public:
    Cache();

    // remove all empty chunks from world data
    void unload() override;

    // world entities
    Chunk &get_chunk(int gx, int gy) override;
    Cell &get_cell(int gx, int gy) override;

    // world data
    std::unordered_map<long long, Chunk> *get_world() override;

private:
    std::unordered_map<long long, Chunk> chunks;
};

#endif