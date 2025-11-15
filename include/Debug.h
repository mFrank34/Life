#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include <vector>
#include <unordered_map>
#include <iostream>

class World;

class Debug
{
private:
    std::vector<World *> worlds;
    int active_index = -1;

public:
    // setting world inspection
    void register_world(World *world);
    void set_active(int index);

    // debugging worlds
    void positions(int global_x, int global_y);
    void all_chunks();

    // alive cell with something like w, r, g, b.
    int active_chunks();

    // total chunks in memory.
    int total_chunks();

    // Optional helpers
    void list_worlds();
};

#endif