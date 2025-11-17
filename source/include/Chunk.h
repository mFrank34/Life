#ifndef LIFE_CHUNK_H
#define LIFE_CHUNK_H

#include "DYNChunk.h"

class Chunk final : public DYNChunk<32>
{
public:
    Chunk(int cx, int cy);
};

#endif //LIFE_CHUNK_H