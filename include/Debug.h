#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <unordered_map>

class Map;   // forward declare
class Chunk; // forward declare

class Debug
{
public:
    Debug() = default;

    // link Debug to its owning Map
    void set(Map *map_ptr);

    // print data
    void positions(int global_x, int global_y);
    void all_chunks(const std::unordered_map<long long, Chunk> chunks);

    // find active chunks within map system
    int active_chunks(const std::unordered_map<long long, Chunk> chunks);

private:
    Map *map = nullptr;
};

#endif