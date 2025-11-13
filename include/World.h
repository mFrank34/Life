#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <vector>
#include "Chunk.h"
#include "Cell.h"


class World
{
public:
    // constructor's and deconstructor's
    World(std::string world_type);
    virtual ~World() = default;

    // Core functions for other map systems
    virtual void unload() = 0;
    virtual Chunk& get_chunk(int global_x, int global_y) = 0;
    virtual Cell& get_cell(int global_x, int global_y) = 0;
    virtual std::unordered_map<long long, Chunk>* get_world() = 0;

    // helper functions for all worlds
    int neighbour_count(int global_x, int global_y);
    std::vector<long long> get_neighbour_key(long long key) const;

    // World information
    std::string get_type() const;

protected:
    static const int KEYLENGTH = 32;

    long long generate_key(int chunk_x, int chunk_y) const;
    std::pair<int, int> decode_key(long long key) const;

    std::string world_type;
};

#endif