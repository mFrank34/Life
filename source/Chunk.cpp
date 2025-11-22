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

Chunk::Chunk(const Chunk& other)
    :chunk_x(other.chunk_x), chunk_y(other.chunk_y), size(other.size), cells(other.cells) {}

Chunk& Chunk::operator=(const Chunk& other)
{
    if (this != &other)
    {
        chunk_x = other.chunk_x;
        chunk_y = other.chunk_y;
        size = other.size;
        cells = other.cells;
    }
    return *this;
}

Chunk::Chunk(Chunk&& other) noexcept
    :chunk_x(other.chunk_x), chunk_y(other.chunk_y), size(other.size), cells(std::move(other.cells)) {}

Chunk& Chunk::operator=(Chunk&& other) noexcept
{
    if (this != &other)
    {
        chunk_x = other.chunk_x;
        chunk_y = other.chunk_y;
        size = other.size;
        cells = std::move(other.cells);
    }
    return *this;
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
    if (cells.size() != size * size)
    {
        // error handle
        std::cerr << "Chunk not initialized Properly!" << std::endl;
    }
    // Print column headers once
    std::cout << "   |";
    for (int x = 0; x < size; x++)
        std::cout << std::setw(2) << x;
    std::cout << '\n';

    // Print a separator line
    std::cout << "---+";
    for (int x = 0; x < size; x++)
        std::cout << "--";
    std::cout << '\n';

    // Print each row with its index and contents
    for (int y = 0; y < size; y++)
    {
        std::cout << std::setw(2) << y << " |";
        for (int x = 0; x < size; x++)
        {
            std::cout << std::setw(2) << cells[y * size + x].get_type();
        }
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