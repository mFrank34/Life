#include "Map.h"
#include <unordered_map>
#include <vector>

Map::Map()
{
    debug.set(this);
};

std::unordered_map<long long, Chunk> Map::get_world()
{
    return chunks;
}

Cell &Map::get_cell(int global_x, int global_y)
{
    int chunk_x = global_x / Chunk::CHUNK_SIZE;
    int chunk_y = global_y / Chunk::CHUNK_SIZE;

    // generating chunk
    Chunk &chunk = generate_chunk(chunk_x, chunk_y);

    // cords maths for x and y
    int local_x = chunk.local_x(global_x);
    int local_y = chunk.local_y(global_y);

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

int Map::neighbour_count(int global_x, int global_y)
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

std::vector<long long> Map::get_neighbor_key(long long key)
{
    std::pair<int, int> cords = decode_key(key); // x and y
    int x = cords.first;
    int y = cords.second;

    std::vector<long long> neighbors; 
    neighbors.reserve(8); 

    // searching for keys to add
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx = 0 && dy == 0) continue; 
            neighbors.push_back(generate_key(x + dx, y + dy));
        }
    }
    return neighbors;
}

void Map::unload()
{
    // get the map data
    std::unordered_map<long long, Chunk>::iterator selected = chunks.begin();
    while (selected != chunks.end())
    {
        // gets chunk
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

void Map::update(Life &rules)
{
    // create buffer for chunks
    std::unordered_map<long long, Chunk> new_chunk;

    for (auto &[key, chunk] : chunks)
    {
        // list of relevant chunks
        std::vector<Chunk*> relevant_chunks;
        relevant_chunks.push_back(&chunk);
        // checking the chunk and its neighbors
        for (auto neighbor_key : get_neighbor_key(key))
        {
            // world contains any keys 
            if (chunks.contains(neighbor_key))
            {
                relevant_chunks.push_back(&chunks[neighbor_key]);
            }
        }
        // michael wright code fo game of life here
        Chunk update_chunk = rules.step(chunk, relevant_chunks);

        // store update chunk
        new_chunk[key] = std::move(update_chunk);
    }
    chunks = std::move(new_chunk);
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

long long Map::generate_key(int chunk_x, int chunk_y) const
{
    // packs two 32 bit ints into one 64-bit key for the map
    return (static_cast<long long>(chunk_x) << 32) | (static_cast<unsigned int>(chunk_y));
}

inline std::pair<int, int> Map::decode_key(long long key)
{
    int chunk_x = static_cast<int>(key >> KEYLENGTH); // decode key 32bit
    int chunk_y = static_cast<int>(key & 0xFFFFFFFF); // -1 32bit
    return {chunk_x, chunk_y};
}