#include <iostream>
#include <vector>

// custom class
#include "Cell.h"
#include "Chunk.h"
#include "Map.h"
#include "Neighbor.h"

/*
NOTES: 
    - so far created a map system that can hold two types of data bool and char file type in 
        class from cell that holds a bool and char, and coors.
    - next steps to create a system for viewing the map 
    - 
*/

/*
Miles Stones:
    - Map system created, able to view a single cell and chunk with global coors.
    - next to make way for node to interact with map, partly done with with cell type and active note.

*/

int main()
{
    Map world;

    int x = 50;
    int y = 50;

    world.get_cell(x, y).set_type('w');
    
    std::cout << "World cell at " << x << " " << y << " Type: "
              << world.get_cell(8, 8).get_type() << "\n";

    world.get_chunk(x,y).print_chunk();

    world.debug_position(x,y);
              
    return 0;
};