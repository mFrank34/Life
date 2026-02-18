/*
 * File: Unordered.h
 * Author: Michael Franks
 * Description:
 */

#pragma once
#ifndef UNORDERED_H
#define UNORDERED_H

// custom classes
#include "world/World.h"
#include "world/structure/Chunk.h"
#include "world/structure/Cell.h"

class Unordered : public World
{
public:
    Unordered();
    ~Unordered() = default;

    // remove all empty chunks from world data
    void unload() override;

    // world entities
    Cell& get_cell(int global_x, int global_y) override;
    Chunk& get_chunk(int gx, int gy) override;
    Chunk& get_chunk(long long key) override;
};

#endif
