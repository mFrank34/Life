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

    world.get_cell(8, 8).set_type('w');
    
    std::cout << "World cell at 8, 8 type: "
              << world.get_cell(8, 8).get_type() << "\n";

    world.get_chunk(8,8).print_chunk();

    world.get_cell(48, 48).set_type('#');

    std::cout << "World cell at 48, 48 type: "
              << world.get_cell(48, 48).get_type() << "\n";

    world.get_chunk(48, 48).print_chunk();
              
    return 0;
};