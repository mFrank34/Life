#include "Life.h"

Life::Life() {};

// Any live cell with fewer than two live neighbours dies, as if by under-population.
bool Life::dies()
{
    
}

// Any live cell with two or three live neighbours lives on to the next generation.
bool Life::lives() 
{

}

// Any live cell with more than three live neighbours dies, as if by overpopulation.
bool Life::overpopulation()
{

}

// Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
bool Life::reproduction()
{

};

Life::~Life() {};
