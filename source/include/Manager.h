#pragma once
#ifndef MANAGER_H
#define MANAGER_H

#include "Manager.h"
#include "Sparse.h"
#include "Rules.h"

struct CellQuery
{
    int chunk_x;
    int chunk_y;
    int cell_x;
    int cell_y;
};

class Manager
{
private:
    // for changing the chunk off set value THIS Must Not CHANGE
    static constexpr int CHUNK_OFF_SET = 2;
    // rules and world objects
    World& world;
    Rules& rules;

    // finding out chunks that active
    std::vector<std::pair<int, long long>> active_neighbour;
    // id and cells that boring the that chunk
    std::vector<std::pair<int, std::vector<Cell>>> neighbour_cells; // id and cells

    // neighbour edge cases
    void find_active_neighbour(std::vector<long long> keys, std::unordered_map<long long, Chunk> selected);
    void Neighbours_cells_edge(std::unordered_map<long long, Chunk> selected_world, int SIZE);

public:
    Manager( World& world,  Rules& rules);
    ~Manager() = default;

    void update();

    void step();
    void render_step();


};



#endif