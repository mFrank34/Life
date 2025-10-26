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

bool Cell::get_node()
{
    // sends back the node type
    return node;
};

void Cell::set_type(char new_type)
{
    // setting the type of node
    this->type = new_type;
    update_cell();
}

void Cell::set_node(bool new_node)
{
    // setting the node state
    this->node = new_node;
}

void Cell::update_cell()
{
    switch (type)
    {
    case '0':
        node = false;
        break; // empty cell
    case 'w':
        node = true;
        break; // white cell
    default:
        node = false;
        break; // default cell type
    }
}

// deconstructor
Cell::~Cell() {};