/*
 * File: Cell.cpp
 * Author: Michael Franks
 * Description:
 */

#include "Cell.h"

// constructor
Cell::Cell() : type(CellType::Empty), alive(false)
{
};

Cell::Cell(CellType type) : type(type)
{
    update_cell();
};

CellType Cell::get_type() const
{
    // returns the type of current cell
    return type;
};

bool Cell::is_alive() const
{
    // sends back the alive type
    return alive;
};

void Cell::set_type(CellType new_type)
{
    // setting the type of alive
    this->type = new_type;
    update_cell();
}

void Cell::set_alive(const bool is_alive)
{
    // setting the alive state
    this->alive = is_alive;
}

void Cell::update_cell()
{
    switch (static_cast<CellType>(type))
    {
    case CellType::Empty:
        alive = false;
        break;

    case CellType::White:
    case CellType::Green:
    case CellType::Blue:
    case CellType::Red:
        alive = true;
        break;

    default:
        alive = false;
        break;
    }
}
