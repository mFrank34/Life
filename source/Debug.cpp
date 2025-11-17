#include "Debug.h"

#include <ranges>
#include "World.h"
#include "DYNChunk.h"
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

void Debug::list_worlds()
{
    std::cout << "Registered worlds:\n";
    for (size_t i = 0; i < worlds.size(); ++i)
    {
        std::cout << "  [" << i << "] " << worlds[i]->get_type() << "\n";
    }
    std::cout << '\n';
}

void Debug::positions(int gx, int gy)
{
    if (active_index == -1)
    {
        std::cerr << "No world set!\n";
        return;
    }

    World *world = worlds[active_index];
    Chunk &chunk = world->get_chunk(gx, gy);

    std::cout << "Global: (" << gx << ", " << gy << ")\n";
    std::cout << "Chunk:  (" << chunk.get_CX() << ", " << chunk.get_CY() << ")\n";
    std::cout << "Alive Cells: " << chunk.populated_amt() << "\n";
    std::cout << "Local:  (" << chunk.get_LX(gx) << ", " << chunk.get_LY(gy) << ")\n";
    std::cout << "Cell Type: '" << chunk.get_cell(gx, gy).get_type() << "'\n\n";
}

void Debug::all_chunks()
{
    if (active_index == -1)
    {
        std::cerr << "No world set!\n";
        return;
    }

    auto *world = worlds[active_index];
    auto *chunks = static_cast<std::unordered_map<long long, Chunk> *>(world->get_world());

    for (const auto &selected : *chunks)
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

    auto *world = worlds[active_index];
    auto *chunks = static_cast<std::unordered_map<long long, Chunk> *>(world->get_world());

    int active = 0;
    for (const auto & val : *chunks | std::views::values)
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

    auto *world = worlds[active_index];
    auto *chunks = static_cast<std::unordered_map<long long, Chunk> *>(world->get_world());
    return chunks->size();
}
