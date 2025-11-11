#include "Debug.h"
#include "Chunk.h"
#include "Cell.h"

void Debug::set(World* world_ptr)
{
    this->world = world_ptr;
}

void Debug::positions(int global_x, int global_y)
{
    if (!world)
    {
        std::cerr << "Debug error: no World set!\n";
        return;
    }

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
    if (!world)
    {
        std::cerr << "Debug error: no World set!\n";
        return;
    }

    // `get_world()` returns void*, so cast it back to the real type
    auto* chunks = static_cast<std::unordered_map<long long, Chunk>*>(world->get_world());
    for (const auto& selected : *chunks)
    {
        long long key = selected.first;
        const Chunk& chunk = selected.second;

        std::cout << "Chunk key: " << key << "\n";
        std::cout << "Chunk: (" << chunk.get_x() << ", " << chunk.get_y() << ")\n";
        chunk.print_chunk();
        std::cout << "\n";
    }
}

int Debug::active_chunks()
{
    if (!world)
    {
        std::cerr << "Debug error: no World set!\n";
        return 0;
    }

    auto* chunks = static_cast<std::unordered_map<long long, Chunk>*>(world->get_world());
    int active = 0;
    for (const auto& selected : *chunks)
    {
        if (selected.second.is_populated())
            active++;
    }
    return active;
}
