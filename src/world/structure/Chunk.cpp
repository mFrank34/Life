/*
 * File: Chunk.cpp
 * Author: Michael Franks
 * Description:
 */

#include "world/structure/Chunk.h"
#include <array>


Chunk::Chunk(const int cx, const int cy, const int cs)
    : chunk_x(cx), chunk_y(cy), size(cs), cells(size * size) {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            get_cell(x,y).set_type('0');
        }
    }
}

int Chunk::get_CX() const
{
    return chunk_x;
}

int Chunk::get_CY() const
{
    return chunk_y;
}

int Chunk::get_LX(const int gx) const
{
    return (gx % size + size) % size;
}

int Chunk::get_LY(const int gy) const
{
    return (gy % size + size) % size;
}

Cell& Chunk::get_cell(const int x, const int y)
{
    return cells[y * size + x];
}

const Cell& Chunk::get_cell(const int x, const int y) const
{
    return cells[y * size + x];
}

Cell& Chunk::get_cell(const int index)
{
    // ONLY use of needing index instead
    return cells[index];
}

const std::vector<Cell>& Chunk::get_cells() const
{
    return cells;
}

bool Chunk::is_populated() const
{
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
            if (cells[y * size + x].is_alive())
                return true;
    }
    return false;
}

int Chunk::populated_amt() const
{
    int lives_cell = 0;
    for (int y = 0; y < size; ++y)
        for (int x = 0; x < size; ++x)
            if (cells[y * size + x].is_alive())
                lives_cell++;
    return lives_cell;
}

int Chunk::neighbour_count(const int cx, const int cy) const
{
    using Offset = std::array<int, 2>;
    using Offsets = std::array<Offset, 8>;

    static const Offsets offsets = {{
        {{-1, -1}}, {{ 0, -1}}, {{ 1, -1}},
        {{-1,  0}},             {{ 1,  0}},
        {{-1,  1}}, {{ 0,  1}}, {{ 1,  1}}
    }};

    int count = 0;
    for (const auto &offset : offsets) // connects
        if (get_cell(cx + offset[0], cy + offset[1]).is_alive())
            count++; //counting the alive cells
    return count;
}

int Chunk::get_size() const
{
    return size;
}