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
    void positions(int gx, int gy) const;
    void all_chunks() const;

    // alive cell with something like w, r, g, b.
    int active_chunks() const;

    // total chunks in memory.
    [[nodiscard]] int total_chunks() const;

    // Optional helpers
    void list_worlds() const;
};

#endif