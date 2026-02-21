/*
 * File: Cell.h
 * Author: Michael Franks
 * Description: cell entry
 */

#pragma once
#ifndef CELL_H
#define CELL_H

enum class CellType
{
    Blue, Red, Green, White, Empty
};

class Cell
{
    CellType type;
    bool alive;

public:
    Cell();
    Cell(CellType type);

    CellType get_type() const;
    bool is_alive() const;
    void set_type(CellType type);
    void set_alive(bool is_alive);
    void update_cell();
    ~Cell() = default;
};

#endif
