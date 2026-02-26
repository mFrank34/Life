/*
* File: Cell.h
 * Author: Michael Franks
 * Description: cell entry
 */

#pragma once
#ifndef CELL_H
#define CELL_H

#include <cstdint>

enum class CellType : uint8_t
{
    Blue, Red, Green, White, Empty
};

class Cell
{
    uint8_t type;

public:
    Cell();
    Cell(CellType type);

    CellType get_type() const;
    bool is_alive() const;
    void set_type(CellType type);
    ~Cell() = default;
};

#endif
