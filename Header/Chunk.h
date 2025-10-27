#ifndef CHUNK_H
#define CHUNK_H

#include "Cell.h"
#include <iostream>
#include <iomanip>

class Chunk
{
public:
    // chunk size be size of chunk
    static const int CHUNK_SIZE = 16;
    // make class
    Chunk();

    Chunk(int chunk_x, int chunk_y);

    // Getter for a specific cell
    Cell &get_cell(int x, int y);
    const Cell &get_cell(int x, int y) const;

    // Gets Chunk X
    int get_x() const;
    // Gets Chunk Y
    int get_y() const;

    // debugging chunks
    void print_chunk();

    // find the amount of populated
    int populated_chunk();

    // true if chunk contains alive cell
    bool is_populated();

    ~Chunk();

private:
    // location cords
    int chunk_x;
    int chunk_y;

    // chunk base
    Cell cells[CHUNK_SIZE][CHUNK_SIZE];
};

#endif