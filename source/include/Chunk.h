#ifndef LIFE_CHUNK_H
#define LIFE_CHUNK_H

#include <vector>

#include "BaseChunk.h"
#include "Cell.h"

class Chunk final : public BaseChunk
{
public:
    Chunk(int cx, int cy);
    // cell instances
    Cell& get_cell(int x, int y) override;
    const Cell& get_cell(int x, int y) const override;
    // view chunk
    void print_chunk() const override;
    // stats of chunks
    bool is_populated() const override;
    int populate_amt() const override;

private:
    static constexpr int size = 32;
    Cell cells [size][size];
};

#endif //LIFE_CHUNK_H