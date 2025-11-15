#include "Sparse.h"
#include <unordered_map>
#include <vector>

Sparse::Sparse() : World("Sparse") {};

inline int Sparse::chunk_index(int v, int size)
{
    return (v >= 0)
               ? v / size
               : (v - (size - 1)) / size;
}

// important locking to one thread
static thread_local Cell DEAD_CELL_MUTABLE;

Cell &Sparse::get_cell(int global_x, int global_y)
{
    int cx = chunk_index(global_x, Chunk::CHUNK_SIZE);
    int cy = chunk_index(global_y, Chunk::CHUNK_SIZE);

    long long key = generate_key(cx, cy);
    auto it = chunks.find(key);
    
    // change the state of cell
    if (it == chunks.end())
    {
        DEAD_CELL_MUTABLE.set_alive(false);
        return DEAD_CELL_MUTABLE;
    }
}

Cell &Sparse::set_cell(int global_x, int global_y)
{
    // TODO: insert return statement here
}

Chunk &Sparse::get_chunk(int global_x, int global_y)
{
    // TODO: insert return statement here
}

Chunk &Sparse::try_get_chunk(int global_x, int global_y)
{
    // TODO: insert return statement here
}

void Sparse::unload()
{
}

std::unordered_map<long long, Chunk> *Sparse::get_world()
{
    return &chunks;
}
