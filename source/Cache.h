#pragma once
#ifndef CACHE_H
#define CACHE_H

#include "World.h"
#include "Chunk.h"
#include "Cell.h"

#include <unordered_map>
#include <list>

class Cache final : public World
{
public:
    Cache(int size, int max);
    // remove all empty chunks from world data
    void unload() override;
    // world entities
    Cell &get_cell(int gx, int gy) override;
    Chunk &get_chunk(int gx, int gy) override;
    Chunk& get_chunk(long long key) override;

    // world data
    std::unordered_map<long long, Chunk>& get_world() override;
    std::unordered_map<long long, Chunk>& get_next_world() override;
    void swap_world() override;

private:
    Chunk& get_cached_chunk(int gx, int gy);

    // cache settings
    const int CHUNK_SIZE;
    int max_active;
    std::list<long long> cached_keys;

    // storing chunks data
    std::unordered_map<long long, Chunk> world;
    std::unordered_map<long long, Chunk> nextWorld;

    // chunk store
    std::unordered_map<long long, Chunk*> active;
};

#endif