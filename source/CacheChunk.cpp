#include "CacheChunk.h"
#include "BaseChunk.h"
#include <iostream>
#include <iomanip>

// cc is cache chunk
CacheChunk::CacheChunk(int ccx, int ccy)
    : BaseChunk(ccx, ccy, 16)
{
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++)
            cells[y][x] = Cell('0');
}

Cell& CacheChunk::get_cell(int x, int y)
{
    return cells[y][x];
}

const Cell& CacheChunk::get_cell(int x, int y) const
{
    return cells[y][x];
}

void CacheChunk::print_chunk() const
{
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
            std::cout << std::setw(2) << cells[y][x].get_type();
        std::cout << '\n';
    }
    std::cout << '\n';
}

bool CacheChunk::is_populated() const
{
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
            if (cells[y][x].is_alive())
                return true;
    }
    return false;
}

int CacheChunk::populate_amt() const
{
    int lives_cell = 0;
    for (int y = 0; y < size; ++y)
        for (int x = 0; x < size; ++x)
            if (cells[y][x].is_alive())
                lives_cell++;
    return lives_cell;
};



