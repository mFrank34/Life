#include "CacheChunk.h"

Cell &CacheBlock::get(int lx, int ly)
{
    return cells[lx][ly];
}

// CacheChunk

CacheChunk::CacheChunk(int cx_, int cy_) : Chunk(cx_, cy_) {}

int CacheChunk::get_local_x(int global_x) const
{
    return global_x - get_x() * SIZE;
}

int CacheChunk::get_local_y(int global_y) const
{
    return global_y - get_y() * SIZE;
}


int CacheChunk::get_block_index(int local) const
{
    return local / CacheBlock::BLOCK_SIZE;
}

int CacheChunk::get_inner_index(int local) const
{
    return local % CacheBlock::BLOCK_SIZE;
}

// Get a CacheBlock (creates if it doesn’t exist)
CacheBlock &CacheChunk::get_block(int block_x, int block_y)
{
    long long key = (static_cast<long long>(block_x) << 32) | static_cast<unsigned int>(block_y);
    return blocks.try_emplace(key, CacheBlock{}).first->second;
}

// Get a cell by global coordinates
Cell &CacheChunk::get_cell(int global_x, int global_y)
{
    int lx = get_local_x(global_x);
    int ly = get_local_y(global_y);

    int bx = get_block_index(lx);
    int by = get_block_index(ly);

    int ix = get_inner_index(lx);
    int iy = get_inner_index(ly);

    CacheBlock &block = get_block(bx, by);
    return block.get(ix, iy);
}

// Returns true if the chunk has any populated blocks
bool CacheChunk::is_populated() const
{
    return !blocks.empty();
}