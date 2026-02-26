/*
* File: Cell.cpp
 * Author: Michael Franks
 * Description:
 */

#include "Cell.h"

Cell::Cell() : type(static_cast<uint8_t>(CellType::Empty))
{
}

Cell::Cell(CellType t) : type(static_cast<uint8_t>(t))
{
}

CellType Cell::get_type() const
{
    return static_cast<CellType>(type);
}

bool Cell::is_alive() const
{
    return type != static_cast<uint8_t>(CellType::Empty);
}

void Cell::set_type(CellType t)
{
    type = static_cast<uint8_t>(t);
}
