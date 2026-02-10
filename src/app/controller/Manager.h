#pragma once
#ifndef MANAGER_H
#define MANAGER_H

#include "world/World.h"
#include "world/structure/Chunk.h"
#include "rules/Rules.h"

enum class haloDirection
{
    Import,
    Export
};

struct haloInfo
{
    int startX, startY;
    int stepX, stepY;
    int count;
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
    std::vector<std::pair<int, long long>> missing_neighbour;

    // id and cells that boring the that chunk and hands id of chunk and references to cells
    std::vector<std::pair<int, std::vector<std::reference_wrapper<Cell>>>> neighbour_cells;

    // neighbor edge cases
    void find_neighbour(const std::array<long long, 8>& keys,
                        const std::unordered_map<long long, Chunk>& map);

    void generate_missing_neighbour();

    void get_neighbours_edge_case(std::unordered_map<long long, Chunk>& selected_world,
                                  int SIZE);

    static auto halo_bridge(Chunk& buffer,
                            const std::vector<std::pair<int, std::vector<std::reference_wrapper<Cell>>>>& cells,
                            int size,
                            haloDirection dir) -> void;

    // constructing a halo chunk
    static void construct_halo(Chunk& buffer,
                               Chunk& selected,
                               const std::vector<std::pair<int, std::vector<std::reference_wrapper<Cell>>>>& cells);

    // chunk updater function
    // selected is one currently being change
    static void chunk_update(Chunk& haloBuffer,
                             Chunk& next,
                             const Chunk& current);

public:
    Manager(World& world, Rules& rules);

    ~Manager() = default;

    void update();
};

#endif
