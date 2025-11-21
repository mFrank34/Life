#ifndef LIFE_CHUNK_H
#define LIFE_CHUNK_H

#include <vector>
#include "Cell.h"

class Chunk
{
public:
    Chunk(int cx, int cy, int cs);

    virtual ~Chunk() = default;

    // chunk coords
    int get_CX() const;
    int get_CY() const;

    // local coords
    int get_LX(int gx) const;
    int get_LY(int gy) const;

    int get_size() const;

    // cell instances
    Cell& get_cell(int x, int y);
    const Cell& get_cell(int x, int y) const;
    // view chunk
    void print_chunk() const;
    // stats of chunks
    bool is_populated() const;
    int populated_amt() const;

private:
    int chunk_x, chunk_y;
    // Chunk size
    int size;
    std::vector<Cell> cells;
};

#endif //LIFE_CHUNK_H