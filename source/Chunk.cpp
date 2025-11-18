#include "Chunk.h"

#include <iomanip>
#include <iostream>

Chunk::Chunk(const int cx, const int cy)
    : BaseChunk(cx, cy, 32)
{
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++)
            cells[y][x] = Cell('0');
}

Cell& Chunk::get_cell(int x, int y)
{
    return cells[y][x];
}

const Cell& Chunk::get_cell(int x, int y) const
{
    return cells[y][x];
}

void Chunk::print_chunk() const
{
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
            std::cout << std::setw(2) << cells[y][x].get_type();
        std::cout << '\n';
    }
    std::cout << '\n';
}

bool Chunk::is_populated() const
{
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
            if (cells[y][x].is_alive())
                return true;
    }
    return false;
}

int Chunk::populate_amt() const
{
    int lives_cell = 0;
    for (int y = 0; y < size; ++y)
        for (int x = 0; x < size; ++x)
            if (cells[y][x].is_alive())
                lives_cell++;
    return lives_cell;
}