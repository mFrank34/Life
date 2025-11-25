#include <gtk/gtk.h> // UI Lib
#include <iostream>

// custom class
#include <Manager.h>
#include "Rules.h"
#include "Cell.h"
#include "Chunk.h"
#include "Debug.h"
// storage containers
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

void block1(World& world)
{
    constexpr int size = 9;
    for (int y = 0; y < size; y++)
        for (int x = 0; x < size; x++)
            world.get_cell(x,y).set_type('w');
}

void block2(World& world)
{
    constexpr int size = 3;
    for (int y = 0; y < size; y++)
        world.get_cell(0,y+1).set_type('w');
}

Debug global_debug;

int main()
{
    constexpr int size = 5;
    Sparse world(size);
    Rules rules;

    Manager life(world, rules);
    global_debug.register_world(nullptr);

    std::cout << "Starting Pos: " << std::endl;
    block2(world);
    global_debug.all_chunks();

    for (int index = 0; index < 3; index++)
    {
        std::cout << "Step: " << index << std::endl;
        life.update();
        //world.unload();
        global_debug.all_chunks();
    }
    return 0;
}