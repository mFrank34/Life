/*
 * File: Manager.h
 * Author: Michael Franks
 * Description: system to change world state, and update cells for world system
 */

#pragma once
#ifndef MANAGER_H
#define MANAGER_H

#include "world/World.h"
#include "world/structure/Chunk.h"
#include "rules/Rules.h"
#include "threading/Scheduler.h"

class Manager
{
private:
    /* Class Settings and connected objects */
    World* world = nullptr;
    Scheduler* scheduler = nullptr;
    Rules& rules;

    static constexpr int CHUNK_OFF_SET = 2;
    std::atomic<bool> update_in_progress = false;

    enum class Cardinal
    {
        N, NE, E, SE,
        S, SW, W, NW
    };

    struct Neighbour
    {
        Cardinal direction;
        long long key;
    };

    struct NeighbourCell
    {
        Cardinal direction;
        std::vector<std::reference_wrapper<Cell>> cells;
    };

    struct HaloTable
    {
        int startX, startY;
        int stepX, stepY;
        int count;
    };

    struct HaloMap
    {
        Cardinal direction;
        HaloTable table;
    };

    /* Functions to update the world system (below) */

    /**
     * search world for chunks and creates missing
     * @param keys keys to be sort after
     * @param map the world
     * @return list of direction and keys of chunks
     */
    std::vector<Neighbour> find_neighbour(
        const std::array<long long, 8>& keys,
        const std::unordered_map<long long, Chunk>& map
    );

    /**
     * System to get the cells around the selected chunk though keys
     * @param neighbours chunks that are needed by the system
     * @param SIZE size of the current world
     * @return direction and cells from other chunks
     */
    std::vector<NeighbourCell> get_edge_case(
        std::vector<Neighbour> neighbours,
        int SIZE
    );

    /**
     * create a halo map though a table with different size
     * @param size size of chunk
     * @return decision map
     */
    static std::array<HaloMap, 8> build_table(int size);

    /**
     * build a new halo chunk with data given
     * @param selected the chunk that needs to be processed
     * @param neighbours the surrounding neighbors
     * @return new halo chunk
     */
    Chunk build_halo(
        Chunk& selected,
        const std::vector<NeighbourCell>& neighbours
    );

    /**
     * Updates a chunk with the rules set
     * @param halo buffered chunk
     * @return returns the next step in
     */
    Chunk chunk_update(const Chunk& halo);

public:
    /**
     * Creates a manager with a rule set
     * @param rules conway's rules
     */
    Manager(Rules& rules);

    ~Manager() = default;

    /**
     * attaches a world to the manager
     * @param world world container
     */
    void attach_world(World& world);

    /**
     * attaches scheduler
     * @param scheduler class scheduler
     */
    void attach_scheduler(Scheduler& scheduler);

    /**
     * Updates the world and process the next step
     */
    void update();
};

#endif
