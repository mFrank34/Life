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
    std::vector<World*> worlds; // track all registered worlds
    int active_index = -1;

public:
    void register_world(World* world);
    void set_active(int index);
    void positions(int global_x, int global_y);
    void all_chunks();
    int active_chunks();

    // Optional helpers
    void list_worlds();
};


#endif