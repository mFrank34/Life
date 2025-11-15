#include <gtk/gtk.h> // UI Lib
#include <iostream>
#include <vector>

// custom class
#include "Cell.h"
#include "Chunk.h"
#include "Debug.h"
#include "World.h"
#include "Unordered.h"
#include "Sparse.h"
#include "Cache.h"

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

Debug global_debug;

int main()
{
    // Unordered 
    Unordered unordered;

    global_debug.list_worlds();

    global_debug.set_active(0);

    // testing setting single point

    int x = 128, y = 128;

    unordered.get_cell(x, y).set_type('w');

    global_debug.positions(x, y);

    global_debug.all_chunks();

    // creating chunk to emtpy vail
    unordered.get_cell(x, y).set_type('0');

    global_debug.positions(x, y);

    // creating a box within map system
    int square_x = 64, square_y = 64;

    std::cout << "Printing to chunks from (" << square_x << "," << square_y << ")\n";

    for (int sx = 0; sx < square_x; ++sx)
    {
        for (int sy = 0; sy < square_y; ++sy)
        {
            unordered.get_cell(sx, sy).set_type('w');
        }
    }

    std::cout << "Active Chunk's: " << global_debug.total_chunks() << "\n";

    unordered.unload();

    std::cout << "Active Chunk After Unload: " << global_debug.total_chunks() << "\n";

    return 0;
}