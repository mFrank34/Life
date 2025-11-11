#include "World.h"
#include <vector>

int World::neighbour_count(int global_x, int global_y) 
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

std::vector<long long> World::get_neighbour_key(long long key) const
{
    auto[x,y] = decode_key(key);
    std::vector<long long> neighbors;
    neighbors.reserve(8);

    // searching for keys to add
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx == 0 && dy == 0)
                continue;
            
            neighbors.push_back(generate_key(x + dx, y + dy));
        }
    }
    return neighbors;
}

// protected
long long World::generate_key(int chunk_x, int chunk_y) const
{
    return (static_cast<long long>(chunk_x) << 32) | (static_cast<unsigned int>(chunk_y));
}

// protected
std::pair<int, int> World::decode_key(long long key) const
{
    int chunk_x = static_cast<int>(key >> KEYLENGTH); // decode key 32bit
    int chunk_y = static_cast<int>(key & 0xFFFFFFFF); // -1 32bit
    return {chunk_x, chunk_y};
}
