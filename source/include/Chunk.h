#pragma once
#ifndef CHUNK_H
#define CHUNK_H

#include "Cell.h"
#include <iostream>
#include <iomanip>

class Chunk
{
public:
    Chunk(int cx = 0, int cy = 0);

    // Getters for individual cells
    Cell &get_cell(int x, int y);
    const Cell &get_cell(int x, int y) const;

    // Chunk coordinates
    int get_CX() const;
    int get_CY() const;

    // Local X & Y coordinates
    int get_LX(int gx) const;
    int get_LY(int gy) const;

    // Utilities
    void print_chunk() const;
    bool is_populated() const;
    int populated_amt() const;

    static const int SIZE = 32;  // fixed chunk size
    virtual ~Chunk() = default;

private:
    Cell cells[SIZE][SIZE];
    int cx;
    int cy;
};

#endif