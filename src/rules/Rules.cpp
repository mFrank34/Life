#include "Rules.h"

Rules::Rules() = default;

// Any live cell with fewer than two live neighbours dies, as if by under-population.
bool Rules::underpopulation(int c) const
{
    if (c < limit)
        return true;
    return false;
}

// Any live cell with two or three live neighbours lives on to the next generation.
bool Rules::survival(int c) const
{
    if (c == min || c == limit)
        return true;
    return false;
}

// Any live cell with more than three live neighbours dies, as if by overpopulation.
bool Rules::overpopulation(int c) const
{
    if (c > limit)
        return true;
    return false;
}

// Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
bool Rules::reproduction(int c) const
{
    if (c == limit)
        return true;
    return false;
};

Rules::~Rules() {};
