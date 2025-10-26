#ifndef CHUNK_H
#define CHUNK_H

#include "Cell.h"

class Chunk
{
public:
    static const int CHUNK_SIZE = 32;

    // make class
    Chunk() {};

    Chunk(int chunk_x, int chunk_y);

    // Getter for a specific cell
    Cell &get_cell(int x, int y);
    const Cell &get_cell(int x, int y) const;

    // Get chunk position in chunk grid
    int get_chunk_x() const;
    int get_chunk_y() const;

private:
    // location cords
    int chunk_x;
    int chunk_y;

    // chunk base
    Cell cells[CHUNK_SIZE][CHUNK_SIZE];
};

#endif