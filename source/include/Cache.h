#pragma once
#ifndef CACHE_H
#define CACHE_H

#include "World.h"
#include "Cell.h"
#include "Chunk.h"
#include <unordered_map>
#include <memory>

class Cache final : public World
{
public:
    Cache();
    // unloads
    void unload() override;
    // chunk & cell
    Chunk &get_chunk(int gx, int gy) override;
    Cell &get_cell(int gx, int gy) override;
    // world data
    std::unordered_map<long long, std::unique_ptr<Chunk>> *get_world() override;

private:
    std::unordered_map<long long, std::unique_ptr<Chunk>> chunks;
};

#endif
