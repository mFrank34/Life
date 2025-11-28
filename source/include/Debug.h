#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include <vector>
#include "Cell.h"

class World;

class Debug
{
    std::vector<World *> worlds;
    int active_index = -1;

    void view_chunk(const std::vector<Cell> &cells, int size) const;
public:
    // setting world inspection
    void register_world(World *world);
    void set_active(int index);
    // debugging worlds
    void positions(int gx, int gy) const;
    void all_chunks() const;
    // alive cell with something like w, r, g, b.
    int active_chunks() const;
    // total chunks in memory.
    int total_chunks() const;
    // Optional helpers
    void list_worlds() const;
};

#endif