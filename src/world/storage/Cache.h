/*
 * File: Cache.h
 * Author: Michael Franks
 * Description:
 */

#pragma once
#ifndef CACHE_H
#define CACHE_H

#include "world/World.h"
#include "world/structure/Chunk.h"
#include "world/structure/Cell.h"

#include <unordered_map>
#include <list>

class Cache final : public World
{
public:
    Cache(int size, int max);
    ~Cache() = default;

    void unload() override;
    void clear_world() override;

    // world entities
    Cell& get_cell(int gx, int gy) override;
    Chunk& get_chunk(int gx, int gy) override;
    Chunk& get_chunk(long long key) override;

private:
    Chunk& get_cached_chunk(int gx, int gy);

    int max_active;
    std::list<long long> cached_keys;

    // chunk store
    std::unordered_map<long long, Chunk*> active;
};

#endif