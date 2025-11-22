#ifndef LIFE_H
#define LIFE_H

#include <Sparse.h>

#include "Chunk.h"
#include "vector"

class Rules
{
public:
    Rules();

    // rules
    bool underpopulation(int c) const;
    bool survival(int c) const;
    bool overpopulation(int c) const;
    bool reproduction(int c) const;

    // step 
    Chunk step(Chunk chunk, std::vector<Chunk*> relevent_chunks);

    ~Rules();

private:
    // limits 
    int limit = 3;
    int min = 2;
};

#endif