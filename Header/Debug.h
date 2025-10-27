#pragma once
#ifndef DEBUG_H
#define DEBUG_H

class Debug
{
public:
    Debug();

    void positions(Map &map, int global_x, int global_y);

    void all_chunks(std::unordered_map<long long, Chunk> chunks);

    ~Debug();

private:
    
};

#endif