#pragma once
#ifndef BASECHUNK_H
#define BASECHUNK_H

#include "Cell.h"
#include <iostream>
#include <iomanip>

template <int SIZE>

class BaseChunk
{
public:
    BaseChunk(int x, int y);
    static int get_SIZE();

    // Getter for a specific cell
    Cell &get_cell(int x, int y);
    const Cell &get_cell(int x, int y) const;

    // Chunk X & Y
    int get_CX() const;
    int get_CY() const;

    // Local X & Y
    int get_LX(int gx);
    int get_LY(int gy);

    // viewing chunks
    void print_chunk() const;
    bool is_populated() const;
    int populated_amt() const;

    virtual ~BaseChunk() = default;

protected:
    Cell cells[SIZE][SIZE];
    int cx = 0, cy = 0;
};



#endif
