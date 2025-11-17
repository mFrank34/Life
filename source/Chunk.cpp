#include "Chunk.h"

Chunk::Chunk(int cx, int cy)
{
    this->cx = cx;
    this->cy = cy;
    // Initialize all cells
    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x)
            cells[y][x] = Cell('0');
}

Cell &Chunk::get_cell(int x, int y)
{
    return cells[y][x];
}

const Cell &Chunk::get_cell(int x, int y) const
{
    return cells[y][x];
}

int Chunk::get_CX() const { return cx; }
int Chunk::get_CY() const { return cy; }

int Chunk::get_LX(int gx) const
{
    int lx = gx - cx * SIZE;
    if (lx < 0) lx += SIZE;
    return lx;
}

int Chunk::get_LY(int gy) const
{
    int ly = gy - cy * SIZE;
    if (ly < 0) ly += SIZE;
    return ly;
}

void Chunk::print_chunk() const
{
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
            std::cout << std::setw(2) << cells[y][x].get_type();
        std::cout << "\n";
    }
    std::cout << "\n";
}

bool Chunk::is_populated() const
{
    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x)
            if (cells[y][x].is_alive())
                return true;
    return false;
}

int Chunk::populated_amt() const
{
    int lives_cell = 0;
    for (int y = 0; y < SIZE; ++y)
        for (int x = 0; x < SIZE; ++x)
            if (cells[y][x].is_alive())
                lives_cell++;
    return lives_cell;
}
