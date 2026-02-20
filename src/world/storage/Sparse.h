/*
 * File: Sparse.h
 * Author: Michael Franks
 * Description:
 */

#pragma once
#ifndef SPARSE_H
#define SPARSE_H

#include <unordered_map>

#include "world/World.h"
#include "world/structure/Chunk.h"
#include "world/structure/Cell.h"

class Sparse final : public World
{
public:
    Sparse(int size);

    // unloads chunks in memory by removing them.
    void unload() override;
    void clear_world() override;

    // get world entities
    Cell& get_cell(int gx, int gy) override;
    Chunk& get_chunk(int gx, int gy) override;
    Chunk& get_chunk(long long key) override;
};

#endif