#include "Debug.h"
#include "Map.h"

Debug::Debug() {};

void Debug::positions(Map &map, int global_x, int global_y)
{
        // grabs the current chunk
        Chunk &chunk = map.get_chunk(global_x, global_y);

        // cords maths for x and y
        int local_x = global_x - chunk.get_x() * Chunk::CHUNK_SIZE;
        int local_y = global_y - chunk.get_y() * Chunk::CHUNK_SIZE;

        Cell &cell = chunk.get_cell(local_x, local_y);
        // read-out
        std::cout << "Global: (" << global_x << ", " << global_y << ")\n";
        std::cout << "Chunk:  (" << chunk.get_x() << ", " << chunk.get_y() << ")\n";
        std::cout << "Alive Cells (" << chunk.populated_chunk() << ") \n";
        std::cout << "Local:  (" << local_x << ", " << local_y << ")\n";
        std::cout << "Cell Type: '" << cell.get_type() << "'\n\n";
}

void Debug::all_chunks(std::unordered_map<long long, Chunk> chunks)
{
        for (std::pair<long long, Chunk> selected : chunks)
        {
                // debugging tool
                long long key = selected.first;
                Chunk chunk = selected.second;
                // readout

                std::cout << "Chunk key:" << key << " " << "\n";
                std::cout << "Chunk:  (" << chunk.get_x() << ", " << chunk.get_y() << ")\n";
                chunk.print_chunk();
                std::cout << "\n";
        }
}

Debug::~Debug() {};
