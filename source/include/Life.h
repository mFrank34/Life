#ifndef LIFE_H
#define LIFE_H

#include "Chunk.h"
#include "vector"

class Life
{
public:
    Life();

    // rules
    bool underpopulation(int c);
    bool survival(int c);
    bool overpopulation(int c);
    bool reproduction(int c);

    // step 
    Chunk step(Chunk chunk, std::vector<Chunk*> relevent_chunks);

    ~Life();

private:
    // limits 
    int limit = 3;
    int min = 2;
};

#endif