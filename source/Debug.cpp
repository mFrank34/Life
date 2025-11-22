#include "Debug.h"

#include <ranges>
#include "World.h"
#include "Cell.h"

void Debug::register_world(World *world)
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

void Debug::list_worlds() const
{
    std::cout << "Registered worlds:\n";
    for (size_t i = 0; i < worlds.size(); ++i)
    {
        std::cout << "  [" << i << "] " << worlds[i]->get_type() << "\n";
    }
    std::cout << '\n';
}

void Debug::positions(int gx, int gy) const
{
    if (active_index == -1)
    {
        std::cerr << "No world set!\n";
        return;
    }

    World *world = worlds[active_index];
    Chunk &chunk = world->get_chunk(gx, gy);

    int local_x = chunk.get_LX(gx);
    int local_y = chunk.get_LY(gy);

    std::cout << "Global: (" << gx << ", " << gy << ")\n";
    std::cout << "Chunk:  (" << chunk.get_CX() << ", " << chunk.get_CY() << ")\n";
    std::cout << "Alive Cells: " << chunk.populated_amt() << "\n";
    std::cout << "Local:  (" << local_x << ", " << local_y << ")\n";
    std::cout << "Cell Type: '" << chunk.get_cell(local_x, local_y).get_type() << "'\n\n";
}

void Debug::all_chunks() const
{
    if (active_index == -1)
    {
        std::cerr << "No world set!\n";
        return;
    }

    auto* world = worlds[active_index];
    auto& chunks = world->get_world();

    for (const auto& [key, chunk] : chunks)
    {
        std::cout << "Chunk key: " << key << "\n";
        chunk.print_chunk();
    }
}

int Debug::active_chunks() const
{
    if (active_index == -1)
    {
        std::cerr << "No world set!\n";
        return 0;
    }

    auto* world = worlds[active_index];
    auto& chunks = world->get_world();

    int active = 0;
    for (const auto & val : chunks | std::views::values)
    {
        if (val.is_populated())
            active++;
    }
    return active;
}

int Debug::total_chunks() const
{
    if (active_index == -1)
    {
        std::cerr << "No world set!\n";
        return 0;
    }

    auto* world = worlds[active_index];
    auto& chunks = world->get_world();

    return chunks.size();
}
