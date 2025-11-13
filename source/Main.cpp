#include <gtk/gtk.h> // UI Lib
#include <iostream>
#include <vector>

// custom class
#include "Cell.h"
#include "Chunk.h"
#include "World.h"
#include "Unordered.h"

/*
NOTES:
    - so far created a map system that can hold two types of data bool and char file type in
        class from cell that holds a bool and char, and coors.
    - next steps to create a system for viewing the map
    - add UI elements for map viewing

Miles Stones:
    - Unordered system created, able to view a single cell and chunk with global coors.
    - next to make way for node to interact with map, partly done with with cell type and active note.
    -
*/

int main()
{
    
    std::cout << "Program ran\n";
    return 0;
}