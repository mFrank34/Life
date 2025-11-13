#include "Debug.h"
#include "World.h"
#include "Chunk.h"
#include "Cell.h"

void Debug::register_world(World* world)
{
    worlds.push_back(world);
    if (active_index == -1) // if no world active yet
        active_index = 0;
}

void Debug::set_active(int index)
{
    if (index >= 0 && index < (int)worlds.size())
    {
        active_index = index;
        std::cout << "Switched active world to index: " << index << "\n";
    }
    else
        std::cerr << "Invalid world index!\n";
    std::cout << '\n';
}

void Debug::list_worlds()
{
    std::cout << "Registered worlds:\n";
    for (size_t i = 0; i < worlds.size(); ++i)
    {
        std::cout << "  [" << i << "] " << worlds[i]->get_type() << "\n";
    }
    std::cout << '\n';
}

void Debug::positions(int global_x, int global_y)
{
    if (active_index == -1)
    {
        std::cerr << "No world set!\n";
        return;
    }

    World* world = worlds[active_index];
    Chunk& chunk = world->get_chunk(global_x, global_y);
    int local_x = chunk.local_x(global_x);
    int local_y = chunk.local_y(global_y);
    Cell& cell = chunk.get_cell(local_x, local_y);

    std::cout << "Global: (" << global_x << ", " << global_y << ")\n";
    std::cout << "Chunk:  (" << chunk.get_x() << ", " << chunk.get_y() << ")\n";
    std::cout << "Alive Cells: " << chunk.populated_chunk() << "\n";
    std::cout << "Local:  (" << local_x << ", " << local_y << ")\n";
    std::cout << "Cell Type: '" << cell.get_type() << "'\n\n";
}

void Debug::all_chunks()
{
    if (active_index == -1)
    {
        std::cerr << "No world set!\n";
        return;
    }

    auto* world = worlds[active_index];
    auto* chunks = static_cast<std::unordered_map<long long, Chunk>*>(world->get_world());

    for (const auto& selected : *chunks)
    {
        std::cout << "Chunk key: " << selected.first << "\n";
        selected.second.print_chunk();
    }
}

int Debug::active_chunks()
{
    if (active_index == -1)
    {
        std::cerr << "No world set!\n";
        return 0;
    }

    auto* world = worlds[active_index];
    auto* chunks = static_cast<std::unordered_map<long long, Chunk>*>(world->get_world());

    int active = 0;
    for (const auto& selected : *chunks)
    {
        if (selected.second.is_populated())
            active++;
    }
    return active;
}
