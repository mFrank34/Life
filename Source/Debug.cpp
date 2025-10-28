#include "Debug.h"
#include "Map.h"
#include "Chunk.h" // if needed

void Debug::set(Map *map_ptr)
{
        this->map = map_ptr;
}

void Debug::positions(int global_x, int global_y)
{
        if (!map)
        {
                std::cerr << "Debug error: no Map set!\n";
                return;
        }

        // get chunk from Map
        Chunk &chunk = map->get_chunk(global_x, global_y);

        // coordinate math
        int local_x = global_x - chunk.get_x() * Chunk::CHUNK_SIZE;
        int local_y = global_y - chunk.get_y() * Chunk::CHUNK_SIZE;

        // get cell
        Cell &cell = chunk.get_cell(local_x, local_y);

        // debug output
        std::cout << "Global: (" << global_x << ", " << global_y << ")\n";
        std::cout << "Chunk:  (" << chunk.get_x() << ", " << chunk.get_y() << ")\n";
        std::cout << "Alive Cells: " << chunk.populated_chunk() << "\n";
        std::cout << "Local:  (" << local_x << ", " << local_y << ")\n";
        std::cout << "Cell Type: '" << cell.get_type() << "'\n\n";
}

void Debug::all_chunks(const std::unordered_map<long long, Chunk> chunks)
{
        for (const auto &selected : chunks)
        {
                long long key = selected.first;
                const Chunk &chunk = selected.second; // note the '&'

                std::cout << "Chunk key: " << key << "\n";
                std::cout << "Chunk: (" << chunk.get_x() << ", " << chunk.get_y() << ")\n";
                chunk.print_chunk();
                std::cout << "\n";
        }
}
