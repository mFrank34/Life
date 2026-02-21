/*
 * File: Chunk.h
 * Author: Michael Franks
 * Description:
 */

#ifndef LIFE_CHUNK_H
#define LIFE_CHUNK_H

#include <vector>
#include "Cell.h"

struct Count
{
    int white = 0;
    int blue = 0;
    int green = 0;
    int red = 0;
};

class Chunk
{
public:
    Chunk(int cx, int cy, int cs);
    virtual ~Chunk() = default;

    // chunk operators
    Chunk(const Chunk& other) = default;
    Chunk& operator=(const Chunk& other) = default;
    Chunk(Chunk&& other) noexcept = default;
    Chunk& operator=(Chunk&& other) noexcept = default;

    // chunk cords
    int get_CX() const;
    int get_CY() const;

    // local cords
    int get_LX(int gx) const;
    int get_LY(int gy) const;

    // cell instances
    Cell& get_cell(int x, int y);
    const Cell& get_cell(int x, int y) const;
    Cell& get_cell(int index);

    // cell data
    const std::vector<Cell>& get_cells() const;

    // lookup
    bool is_populated() const;
    int populated_amt() const;
    Count neighbour_count(int cx, int cy) const;
    int get_size() const;

private:
    int chunk_x, chunk_y, size;
    std::vector<Cell> cells;
};

#endif //LIFE_CHUNK_H