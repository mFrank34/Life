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
    Scheduler& scheduler;
    Rules& rules;

    static constexpr int CHUNK_OFF_SET = 2;

    /* Thread-safe result accumulation */
    using ChunkResult = std::pair<long long, Chunk>;
    std::vector<ChunkResult> results;
    std::mutex result_mtx;

    /**
     * Cardinal direction store
     */
    enum class Cardinal
    {
        N, NE, E, SE,
        S, SW, W, NW
    };

    /**
     * Stores a direction and the key of the neighbouring chunk
     */
    struct Neighbour
    {
        Cardinal direction;
        long long key;
    };

    /**
     * Stores a direction and OWNED copies of the relevant border cells.
     * Using owned Cell values (not references) makes this safe to pass
     * across thread boundaries without any lifetime concerns.
     */
    struct NeighbourCell
    {
        Cardinal direction;
        std::vector<Cell> cells; // owned copies, not reference_wrappers
    };

    /**
     * Halo table columns — describes how to map one neighbour direction
     * into the halo buffer.
     */
    struct HaloTable
    {
        int startX, startY;
        int stepX, stepY;
        int count;
    };

    /**
     * A complete entry in the halo lookup table.
     */
    struct HaloMap
    {
        Cardinal direction;
        HaloTable table;
    };

    /**
     * All data a worker thread needs to process one chunk.
     * Every field is self-contained — no pointers into shared structures.
     */
    struct ChunkWork
    {
        long long key;
        Chunk chunk_copy; // snapshot of the chunk
        std::vector<NeighbourCell> neighbour_cells; // owned border cells
        int size; // chunk size at capture time
    };

    /* ------------------------------------------------------------------ */
    /*  Internal helpers (all called on the main thread before scheduling) */
    /* ------------------------------------------------------------------ */

    /**
     * Returns Neighbour descriptors for the 8 surrounding chunks.
     * Called on the main thread only.
     */
    std::vector<Neighbour> find_neighbour(
        const std::array<long long, 8>& keys,
        const std::unordered_map<long long, Chunk>& map
    );

    /**
     * Collects and COPIES the border cells from each neighbouring chunk.
     * Called on the main thread only; returns owned Cell values so the
     * result is safe to move into a worker thread.
     */
    std::vector<NeighbourCell> get_edge_case(
        const std::vector<Neighbour>& neighbours,
        int SIZE
    );

    /**
     * Builds the halo-index lookup table for a given chunk size.
     */
    static std::array<HaloMap, 8> build_table(int size);

    /**
     * Builds a (size+2)×(size+2) halo chunk from a snapshot and its
     * pre-collected border cells.  Called from worker threads.
     */
    Chunk build_halo(
        Chunk& selected,
        const std::vector<NeighbourCell>& neighbours,
        int size
    );

    /**
     * Applies Conway's rules to the halo chunk and returns the next
     * generation.  Called from worker threads.
     */
    Chunk chunk_update(const Chunk& halo, int size);

public:
    Manager(Rules& rules, Scheduler& scheduler);
    ~Manager() = default;

    /**
     * Attaches a world to the manager.
     */
    void attach_world(World& world);

    /**
     * Advances the world by one generation.
     */
    void update();
};

#endif
