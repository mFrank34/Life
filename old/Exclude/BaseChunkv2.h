#ifndef BASECHUNK_H
#define BASECHUNK_H

#include "../../source/include/Cell.h"

class BaseChunk
{
public:
    BaseChunk(int cx, int cy, int size);
    virtual ~BaseChunk() = default;

    // chunk coords
    int get_CX() const;
    int get_CY() const;

    // local coords
    int get_LX(int gx);
    int get_LY(int gy);

    int get_size();

    // get the cells within elements / chunks
    virtual Cell &get_cell(int x, int y) = 0;
    virtual const Cell &get_cell(int x, int y) const = 0;

    // Utilities for chunk
    virtual void print_chunk() const = 0;
    virtual bool is_populated() const = 0;
    virtual int populated_amt() const = 0;

protected:
    int cx, cy;
    int size;
};

#endif //BASECHUNK_H