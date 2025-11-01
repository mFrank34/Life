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
bool Life::reproduction(int c) {
    if (c == limit)
        return true;
    return false;
};

Chunk Life::step(Chunk chunk, std::vector<Chunk*> relevent_chunks)
{
    




};

Life::~Life() {};
