#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <unordered_map>
#include "World.h"

class Debug
{
public:
    Debug() = default;

    // link Debug to its owning Unordered
    void set(World *world_ptr);

    // print data
    void positions(int global_x, int global_y);
    void all_chunks();
    int active_chunks();

private:
    World *world = nullptr;
};

#endif