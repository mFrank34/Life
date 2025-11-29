#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include <vector>
#include "Cell.h"

class World;

class Debug
{
public:
    void register_world(World *world);
    void set_active(int index);
    void positions(int gx, int gy) const;
    void all_chunks() const;
    int active_chunks() const;
    int total_chunks() const;
    void list_worlds() const;

private:
    std::vector<World *> worlds;
    int active_index = -1;

    static void view_chunk(const std::vector<Cell> &cells, int size);
};

#endif