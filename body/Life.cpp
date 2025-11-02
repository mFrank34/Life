#include "Life.h"

Life::Life() {};

// Any live cell with fewer than two live neighbours dies, as if by under-population.
bool Life::underpopulation(int c)
{
    if (c < limit)
        return true;
    return false;
}

// Any live cell with two or three live neighbours lives on to the next generation.
bool Life::survival(int c)
{
    if (c == min || c == limit)
        return true;
    return false;
}

// Any live cell with more than three live neighbours dies, as if by overpopulation.
bool Life::overpopulation(int c)
{
    if (c > limit)
        return true;
    return false;
}

// Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
bool Life::reproduction(int c)
{
    if (c == limit)
        return true;
    return false;
};

Chunk Life::step(Chunk chunk, std::vector<Chunk *> relevent_chunks)
{
    bool top = false;
    bool bottom = false;
    bool left = false;
    bool right = false;

    for (auto neighbor : relevent_chunks)
    {
        if (!neighbor)
            continue;

        // Check top edge
        for (int col = 0; col < Chunk::CHUNK_SIZE; ++col)
            if (neighbor->get_cell(0, col).is_alive())
                top = true;

        // Check bottom edge
        for (int col = 0; col < Chunk::CHUNK_SIZE; ++col)
            if (neighbor->get_cell(Chunk::CHUNK_SIZE - 1,col).is_alive())
                bottom = true;

        // Check left edge
        for (int row = 0; row < Chunk::CHUNK_SIZE; ++row)
            if (neighbor->get_cell(row, 0).is_alive())
                left = true;

        // Check right edge
        for (int row = 0; row < Chunk::CHUNK_SIZE; ++row)
            if (neighbor->get_cell(row, Chunk::CHUNK_SIZE - 1).is_alive())
                right = true;
    }

    // Now you know which edges have active neighbors
    // You can use this info to update `chunk`'s border cells

    return chunk;
};

Life::~Life() {};
