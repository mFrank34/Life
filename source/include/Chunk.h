#ifndef LIFE_CHUNK_H
#define LIFE_CHUNK_H

#include <vector>
#include "Cell.h"

class Chunk
{
public:
    Chunk(int cx, int cy, int cs);
    virtual ~Chunk() = default;

    // chunk operators
    Chunk(const Chunk& other);
    Chunk& operator=(const Chunk& other);
    Chunk(Chunk&& other) noexcept;
    Chunk& operator=(Chunk&& other) noexcept;

    // chunk coords
    int get_CX() const;
    int get_CY() const;
    // local coords
    int get_LX(int gx) const;
    int get_LY(int gy) const;

    // size
    int get_size() const;
    // cell instances
    Cell& get_cell(int x, int y);
    const Cell& get_cell(int x, int y) const;
    Cell& get_cell(int index);

    // view chunk
    void print_chunk() const;

    // lookup
    bool is_populated() const;
    int populated_amt() const;
    int neighbour_count(int cx, int cy) const;

private:
    int chunk_x, chunk_y;
    // Chunk size
    int size;
    std::vector<Cell> cells;
};

#endif //LIFE_CHUNK_H