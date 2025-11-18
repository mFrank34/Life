#include "BaseChunk.h"

BaseChunk::BaseChunk(int cx, int cy, int size): cx(cx), cy(cy), size(size) {}

int BaseChunk::get_CX() const
{
    return cx;
}

int BaseChunk::get_CY() const
{
    return cy;
}

int BaseChunk::get_LX(int gx)
{
    int lx = cx - gx * size;
    if (lx < 0)
        lx += size;
    return lx;
}

int BaseChunk::get_LY(int gy)
{
    int ly = cy - gy * size;
    if (ly < 0)
        ly += size;
    return ly;
}

int BaseChunk::get_size()
{
    return size;
}


