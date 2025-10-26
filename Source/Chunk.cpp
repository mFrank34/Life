#include "Chunk.h"
#include "Cell.h"

// creates class    | sets the cords for chunk
Chunk::Chunk(int chunk_x, int chunk_y) : chunk_x(chunk_x), chunk_y(chunk_y)
{
    // initialize all cells
    for (int y = 0; y < CHUNK_SIZE; y++)
    {
        // generates y
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            // generates x
            cells[y][x] = Cell('.');
        }
    }
}

// returns reference to modifiable cell
Cell &Chunk::get_cell(int x, int y)
{
    return cells[x][y];
}

// returns const reference (read only access)
const Cell &Chunk::get_cell(int x, int y) const
{
    return cells[x][y];
}

// chuck coordinates getters
int Chunk::get_chunk_x() const
{
    return chunk_x;
}

int Chunk::get_chunk_y() const
{
    return chunk_y;
}