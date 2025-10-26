#include <iostream>
#include <vector>

// custom class
#include "Cell.h"
#include "Chunk.h"
#include "Map.h"
#include "Point.h"
// #include "App.h"

/*
NOTES: 
    - so far created a map system that can hold two types of data bool and char file type in 
        class from cell that holds a bool and char, and coors.
    - next steps to create a system for viewing the map
    - 
*/

int main()
{
    // world you know
    Map world;

    world.get_cell(100, 50).set_type('w');
    
    std::cout << "World cell at 100, 50 type: "
              << world.get_cell(100, 50).get_type() << "\n";

    world.get_cell(200, -50).set_type('#');
    std::cout << "World cell at 200, -50 type: "
              << world.get_cell(200, -50).get_type() << "\n";
              
    return 0;
};