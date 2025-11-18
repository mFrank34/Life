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
    Chunk &get_chunk(int gx, int gy) override;
    Cell &get_cell(int gx, int gy) override;
    // world data
    std::unordered_map<long long, Chunk> *get_world() override;

private:
    Chunk& get_cached_chunk(int gx, int gy);
    // cache settings
    const int CHUNK_SIZE;
    int max_activa;
    std::list<long long> cached_keys;
    // storing chunks data
    std::unordered_map<long long, Chunk> chunks;
    std::unordered_map<long long, Chunk*> active;
};

#endif