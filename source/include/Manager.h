#pragma once
#ifndef MANAGER_H
#define MANAGER_H

#include "World.h"
#include "Rules.h"

enum class haloDirection
{
    Import,
    Export
};

class Manager
{
    // for changing the chunk off set value THIS Must Not CHANGE
    static constexpr int CHUNK_OFF_SET = 2;
    // rules and world objects
    World& world;
    Rules& rules;

    // finding out chunks that active
    std::vector<std::pair<int, long long>> active_neighbour;
    // id and cells that boring the that chunk and hands id of chunk and references to cells
    std::vector<std::pair<int, std::vector<std::reference_wrapper<Cell>>>> neighbour_cells;

    // neighbour edge cases
    void find_active_neighbour(const std::array<long long, 8>& keys,
        const std::unordered_map<long long, Chunk>& selected);
    void neighbours_cells_edge(const std::unordered_map<long long, Chunk>& selected_world, int SIZE);

    // halo chunk helper
    static void halo_bridge(Chunk& buffer,
        const std::vector<std::pair<int, std::vector<std::reference_wrapper<Cell>>>>& cells,
        int size, haloDirection dir);
    // constructing a halo chunk
    static void construct_halo(Chunk& buffer, Chunk& selected,
        const std::vector<std::pair<int, std::vector<std::reference_wrapper<Cell>>>>& cells);
    // chunk updater function
    static void chunk_update(Chunk& buffer, Chunk& selected); // selected is one currently being change

public:
    Manager( World& world,  Rules& rules);
    ~Manager() = default;

    void update();
    static void step();
    static void render_step();
};

#endif