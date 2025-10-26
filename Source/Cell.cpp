#include "Cell.h"
#include <iostream>

// constructor
Cell::Cell() {};

Cell::Cell(char type) : type(type) 
{
    update_cell();
};

char Cell::get_type()
{
    // returns the type of current cell
    return type;
};

bool Cell::is_alive()
{
    // sends back the alive type
    return alive;
};

void Cell::set_type(char new_type)
{
    // setting the type of alive
    this->type = new_type;
    update_cell();
}

void Cell::is_alive(bool is_alive)
{
    // setting the alive state
    this->alive = is_alive;
}

void Cell::update_cell()
{
    switch (type)
    {
    case '0':
        alive = false;
        break; // empty cell
    case 'w':
        alive = true;
        break; // white cell
    default:
        alive = false;
        break; // default cell type
    }
}

// deconstructor
Cell::~Cell() {};