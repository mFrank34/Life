#include <gtk/gtk.h> // UI Lib
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
    - add UI elements for map viewing

Miles Stones:
    - Map system created, able to view a single cell and chunk with global coors.
    - next to make way for node to interact with map, partly done with with cell type and active note.
    - 
*/

int main()
{
    Map world;

    int x = 50;
    int y = 50;

    world.get_cell(x, y).set_type('w');

    std::cout << "World cell at " << x << " " << y << " Type: "
              << world.get_cell(x, y).get_type() << "\n";

    world.get_cell(100, 100).set_type('0');

    world.debug.positions(x, y);

    world.debug.all_chunks();

    std::cout << "Unloading... \n";

    // debugging
    world.unload();

    std::cout << "Printing Chunks in unodered map\n";

    world.debug.all_chunks();

    // creating a box within map system
    int square_x = 16;
    int square_y = 16;

    std::cout << "Printing to chunks from " << square_x << " " << square_y << "\n\n";

    for (int sx = 0; sx < square_x; ++sx)
    {
        for (int sy = 0; sy < square_y; ++sy)
        {
            world.get_cell(sx, sy).set_type('w');
        }
    }

    // active chunks amount
    std::cout << "Active Chunks: " << world.debug.active_chunks() << "\n";

    world.debug.all_chunks();

    // unload chunks that not full
    std::cout << "Unloading... \n";
    world.unload();

    std::cout << "print currents chunks in memory\n\n";

    // display chunks again
    world.debug.all_chunks();

    // active chunks amount
    std::cout << "Active Chunks: " << world.debug.active_chunks() << "\n";

    std::cout << "End Program 1\n";

    world.debug.all_chunks();

    return 0;
}