/*
 * File: World.h
 * Author: Michael Franks
 * Description:
 */

#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <array>
#include <memory>
#include <string>

#include "world/structure/Chunk.h"
#include "world/structure/Cell.h"

class World
{
public:
    // constructor's and deconstructor's
    World(const std::string_view& world_type, int chunk_size);
    virtual ~World() = default;

    // Core functions for other map systems
    virtual void unload() = 0;
    virtual void clear_world() = 0;
    virtual Chunk& get_chunk(int global_x, int global_y) = 0;
    virtual Chunk& get_chunk(long long key) = 0;
    virtual Cell& get_cell(int global_x, int global_y) = 0;

    /* function that stay same though out the system */
    virtual std::unordered_map<long long, Chunk>& get_world();
    virtual std::unordered_map<long long, Chunk>& get_stepped_world();

    virtual void swap_world();

    // helper functions for all worlds
    std::array<long long, 8> get_neighbour_key(long long key) const;

    // World information
    std::string get_type() const;
    int get_size() const;

    // tick system
    virtual void tick(float delta);

protected:
    const int CHUNK_SIZE;
    const std::string TYPE;
    static constexpr int KEY_LENGTH = 32;

    /* Shared World state */
    std::unordered_map<long long, Chunk> world;
    std::unordered_map<long long, Chunk> step;

    /* creating keys for chunks */
    static long long generate_key(int chunk_x, int chunk_y);
    static std::pair<int, int> decode_key(long long key);

    // handing negs
    static int floor_div(int cord, int size);

};

#endif