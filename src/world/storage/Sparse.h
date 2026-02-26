/*
* File: Sparse.h
 * Author: Michael Franks
 * Description:
 */

#pragma once
#ifndef SPARSE_H
#define SPARSE_H

#include "world/World.h"
#include <shared_mutex>

class Sparse : public World
{
public:
    Sparse(int size);

    Cell&  get_cell(int gx, int gy) override;
    Chunk& get_chunk(int gx, int gy) override;
    Chunk& get_chunk(long long key) override;

    void unload()      override;
    void clear_world() override;

private:
    mutable std::shared_mutex chunk_mtx;
};

#endif