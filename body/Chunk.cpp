#include "Chunk.h"
#include "Cell.h"
#include "Life.h"

// creates class | sets the cords for chunk
Chunk::Chunk(int input_x, int input_y) : chunk_x(input_x), chunk_y(input_y)
{
    // initialize all cells
    for (int y = 0; y < SIZE; y++)
    {
        // generates y
        for (int x = 0; x < SIZE; x++)
        {
            // generates x
            cells[y][x] = Cell('0');
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

/* Gets Chunk X*/
int Chunk::get_x() const
{
    return chunk_x;
}

/* Gets Chunk Y*/
int Chunk::get_y() const
{
    return chunk_y;
}

/* Gets Local X*/
int Chunk::local_x(int global_x)
{
    return global_x - get_x() * Chunk::SIZE;
}

/* Gets Local Y*/
int Chunk::local_y(int global_y)
{
    return global_y - get_y() * Chunk::SIZE;
}

void Chunk::print_chunk() const
{
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            // prints data that is stored within said chunk
            std::cout << std::setw(2) << get_cell(x, y).get_type();
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int Chunk::populated_chunk() const 
{
    int lives_cell;
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            if (get_cell(x, y).is_alive())
            {
                lives_cell++;
            }
        }
    }
    return lives_cell;
}

bool Chunk::is_populated() const
{
    // search
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            if (get_cell(x, y).is_alive())
            {
                // if a live cell within chunk don't matter where returns true
                return true;
            }
        }
    }
    // if none are alive returns false
    return false;
}

Chunk::~Chunk() {};
