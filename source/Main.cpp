#include <gtk/gtk.h> // UI Lib
#include <iostream>
#include <vector>

// custom class
#include <Manager.h>

#include "Rules.h"
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
    - hey michael fix the cache map system

Miles Stones:
    - Unordered system created, able to view a single cell and chunk with global coors.
    - next to make way for node to interact with map, partly done with cell type and active note.
    - Create unordered and sparse map systems
*/

Debug global_debug;

int main()
{
    constexpr int size = 3;
    Sparse world(size);
    Rules rules;

    Manager life(world, rules);
    global_debug.register_world(0);

    global_debug.all_chunks();
    //global_debug.all_chunks();

    for (int index = 0; index < size; index++)
    {
        std::cout << "Index:" << index << std::endl;
        life.update();
        world.unload();
        global_debug.all_chunks();
    }
    return 0;
}