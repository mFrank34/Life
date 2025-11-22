#include "Chunk.h"
#include <iomanip>
#include <iostream>

Chunk::Chunk(const int cx, const int cy, int const cs)
    : chunk_x(cx), chunk_y(cy), size(cs)
{
    cells.resize(size * size);
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++)
            cells[y * size + x] = Cell('0');
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

int Chunk::get_size() const
{
    return size;
}

Cell& Chunk::get_cell(const int x, const int y)
{
    return cells[y * size + x];
}

const Cell& Chunk::get_cell(const int x, const int y) const
{
    return cells[y * size + x];
}

void Chunk::print_chunk() const
{
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
            std::cout << std::setw(2) << cells[y * size + x].get_type();
        std::cout << '\n';
    }
    std::cout << '\n';
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