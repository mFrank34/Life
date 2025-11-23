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
public:
    Manager( World& world,  Rules& rules);
    ~Manager() = default;

    void update();
    void step();
    void render_step();

private:
    // private Logic
    void Neighbours_cells_edges(auto&);

    void find_active_neighbour(auto& keys, auto&);

    // for changing the chunk off set value THIS Must Not CHANGE
    static constexpr int CHUNK_OFF_SET = 2;
    // finding out chunks that active
    std::vector<std::pair<int, long long>> active_neighbour; // id and key
    // id and cells that boring the that chunk
    std::vector<std::pair<int, std::vector<Cell>>> neighbour_cells; // id and cell

    World& world;
    Rules& rules;
};



#endif