#include "Debug.h"

#include <ranges>
#include <format>
#include <iostream>

#include "World.h"
#include "Cell.h"

void Debug::view_chunk(const std::vector<Cell> &cells, const int size) {
    if (cells.size() != size * size)
    {
        std::cerr << "Chunk not initialized properly!\n";
        return;
    }

    // Column headers
    std::string header = "   |";
    for (int x = 0; x < size; ++x)
        header += std::format("{:2}", x);
    std::cout << header << '\n';

    // Separator line
    std::string sep = "---+";
    for (int x = 0; x < size; ++x)
        sep += "--";
    std::cout << sep << '\n';

    // Each row
    for (int y = 0; y < size; ++y)
    {
        std::string row = std::format("{:2} |", y);
        for (int x = 0; x < size; ++x)
        {
            row += std::format("{:2}", cells[y * size + x].get_type());
        }
        std::cout << row << '\n';
    }
    std::cout << '\n';
}

void Debug::register_world(World *world)
{
    worlds.push_back(world);
    if (active_index == -1) // if no world active yet
        active_index = 0;
}

void Debug::set_active(int index)
{
    if (index >= 0 && index < static_cast<int>(worlds.size()))
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
    const auto& chunks = world->get_world();

    for (const auto& [key, chunk] : chunks)
    {
        std::cout << "Chunk key: " << key << "\n";
        view_chunk(chunk.get_cells(), chunk.get_size());
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
    return static_cast<int>(world->get_world().size());
}