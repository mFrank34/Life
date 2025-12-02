#include <gtk/gtk.h>
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
        world.get_cell(2,y+1).set_type('w');
}

Debug global_debug;
int main()
{
    constexpr int size = 5;
    Sparse world(size);
    Rules rules;

    Manager life(world, rules);
    global_debug.register_world(0);

    std::cout << "Starting Pos: " << std::endl;
    block2(world);
    global_debug.all_chunks();

    for (int index = 0; index < 3; index++)
    {
        std::cout << "Step: " << index << std::endl;
        life.update();
        world.unload();
        global_debug.all_chunks();
    }
    return 0;
}