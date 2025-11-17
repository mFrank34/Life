#pragma once
#ifndef DYNCHUNK_H
#define DYNCHUNK_H

#include "Cell.h"
#include <iostream>
#include <iomanip>

/**
 * THIS IS CLASS FOR MAKING A CHUNK FOR 32 AND 16
 * REASON FOR THIS MICHAEL IS FOR CACHE AWARE GRIDS ONLY!
 * 32 IS REGULAR DEFAULT FOR THIS CLASS
 * 16 ONLY FOR CACHE AWARE GRIDS
 *
 * @tparam SIZE THE SIZE OF A CHUNK ONLY NEEDING TWO SIZE 16 AND 32
 */

template<int SIZE>
class DYNChunk
{
public:
    explicit DYNChunk(int cx = 0, int cy = 0)
    {
        this->cx = cx;
        this->cy = cy;
        // Initialize all cells
        for (int y = 0; y < SIZE; ++y)
            for (int x = 0; x < SIZE; ++x)
                cells[y][x] = Cell('0');
    };

    // Getters for individual cells
    Cell &get_cell(int x, int y)
    {
        return cells[y][x];
    };

    [[nodiscard]] const Cell &get_cell(int x, int y) const
    {
        return cells[y][x];
    };

    // Chunk coordinates
    [[nodiscard]] int get_CX() const
    {
        return cx;
    };

    [[nodiscard]] int get_CY() const
    {
        return cy;
    };

    // Local X & Y coordinates
    [[nodiscard]] int get_LX(int gx) const
    {
        int lx = gx - cx * SIZE;
        if (lx < 0) lx += SIZE;
        return lx;
    };

    [[nodiscard]] int get_LY(int gy) const
    {
        int ly = gy - cy * SIZE;
        if (ly < 0) ly += SIZE;
        return ly;
    }

    // Utilities
    void print_chunk() const
    {
        for (int y = 0; y < SIZE; ++y)
        {
            for (int x = 0; x < SIZE; ++x)
                std::cout << std::setw(2) << cells[y][x].get_type();
            std::cout << "\n";
        }
        std::cout << "\n";
    };
    [[nodiscard]] bool is_populated() const
    {
        for (int y = 0; y < SIZE; ++y)
            for (int x = 0; x < SIZE; ++x)
                if (cells[y][x].is_alive())
                    return true;
        return false;
    };

    [[nodiscard]] int populated_amt() const
    {
        int lives_cell = 0;
        for (int y = 0; y < SIZE; ++y)
            for (int x = 0; x < SIZE; ++x)
                if (cells[y][x].is_alive())
                    lives_cell++;
        return lives_cell;
    };

    static int get_SIZE()
    {
        return SIZE;
    }

    //static constexpr int SIZE;  // fixed chunk size
    virtual ~DYNChunk() = default;

private:
    Cell cells[SIZE][SIZE];
    int cx = 0, cy = 0;
};

#endif
