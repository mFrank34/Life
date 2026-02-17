/*
 * File: Unordered.h
 * Author: Michael Franks
 * Created: 2026-02-17
 * Last Edited: 2026-02-10
 * Description:
 */

#pragma once
#ifndef UNORDERED_H
#define UNORDERED_H

#include <unordered_map>

// custom classes
#include "world/World.h"
#include "world/structure/Chunk.h"
#include "world/structure/Cell.h"

class Unordered : public World
{
public:
    Unordered(int size);

    // remove all empty chunks from world data
    void unload() override;

    // world entities
    Cell &get_cell(int global_x, int global_y) override;
    Chunk &get_chunk(int gx, int gy) override;
    Chunk &get_chunk(long long key) override;
    void swap_world() override;

    // returning world data
    std::unordered_map<long long, Chunk>& get_world() override;
    std::unordered_map<long long, Chunk>& get_next_world() override;

private:
    // system setup
    const int CHUNK_SIZE;

    // world data
    std::unordered_map<long long, Chunk> world;
    std::unordered_map<long long, Chunk> nextWorld;

};

#endif