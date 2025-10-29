#include "Chunk.h"
#include "Cell.h"

// creates class    | sets the cords for chunk
Chunk::Chunk(int input_x, int input_y) : chunk_x(input_x), chunk_y(input_y)
{
    // initialize all cells
    for (int y = 0; y < CHUNK_SIZE; y++)
    {
        // generates y
        for (int x = 0; x < CHUNK_SIZE; x++)
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

void Chunk::print_chunk() const
{
    for (int y = 0; y < CHUNK_SIZE; ++y)
    {
        for (int x = 0; x < CHUNK_SIZE; ++x)
        {
            // prints data that is stored within said chunk
            std::cout << std::setw(2) << get_cell(x, y).get_type();
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int Chunk::populated_chunk()
{
    int lives_cell;
    for (int y = 0; y < CHUNK_SIZE; ++y)
    {
        for (int x = 0; x < CHUNK_SIZE; ++x)
        {
            if (get_cell(x, y).is_alive())
            {
                lives_cell++;
            }
        }
    }
    return lives_cell;
}

bool Chunk::is_populated()
{
    // search
    for (int y = 0; y < CHUNK_SIZE; ++y)
    {
        for (int x = 0; x < CHUNK_SIZE; ++x)
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
