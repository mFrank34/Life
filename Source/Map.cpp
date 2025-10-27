#include "Map.h"
#include "Debug.h"

long long Map::generate_key(int chunk_x, int chunk_y) const
{
    // packs two 32 bit ints into one 64-bit key for the map
    return (static_cast<long long>(chunk_x) << 32) | (static_cast<unsigned int>(chunk_y));
}

Chunk &Map::generate_chunk(int chunk_x, int chunk_y)
{
    long long key = generate_key(chunk_x, chunk_y);
    // if the chunk doesn't exist, create new chunk
    if (chunks.find(key) == chunks.end())
    {
        chunks.emplace(key, Chunk(chunk_x, chunk_y));
    };
    // creates chunk
    return chunks.at(key);
}

Cell &Map::get_cell(int global_x, int global_y)
{
    int chunk_x = global_x / Chunk::CHUNK_SIZE;
    int chunk_y = global_y / Chunk::CHUNK_SIZE;

    // generating chunk
    Chunk &chunk = generate_chunk(chunk_x, chunk_y);

    // cords maths for x and y
    int local_x = global_x - chunk_x * Chunk::CHUNK_SIZE;
    int local_y = global_y - chunk_y * Chunk::CHUNK_SIZE;

    // return the cell
    return chunk.get_cell(local_x, local_y);
}

Chunk &Map::get_chunk(int global_x, int global_y)
{
    // reversed the cell function to return chunk as well
    int chunk_x = global_x / Chunk::CHUNK_SIZE;
    int chunk_y = global_y / Chunk::CHUNK_SIZE;
    // return the chunk
    return generate_chunk(chunk_x, chunk_y);
}

int Map::number_live(int global_x, int global_y)
{
    // offset for cords
    static const int offsets[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    int count;

    for (const auto &offset : offsets) // connects
    {
        // gets the surrounding cell coors
        Cell &neighbor = get_cell(global_x + offset[0], global_y + offset[1]);
        if (neighbor.is_alive())
        {
            // counting the alive cells
            count++;
        };
    };
    return count;
}

void Map::debug_position(int global_x, int global_y)
{
    // grabs the current chunk
    Chunk &chunk = get_chunk(global_x, global_y);

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

void Map::print_all_chunks()
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

void Map::unload()
{
    // get the map data
    std::unordered_map<long long, Chunk>::iterator selected = chunks.begin();
    while (selected != chunks.end())
    {
        // gets key & chunk
        long long key = selected->first;
        Chunk &chunk = selected->second;

        // finds if data is stored with in
        if (!chunk.is_populated()) // if not populated
        {
            selected = chunks.erase(selected);
        }
        else
        {
            ++selected;
        }
    }
}
