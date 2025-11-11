#pragma once
#ifndef WORLD_H
#define WORLD_H

#include "Chunk.h"
#include "Cell.h"
#include "Debug.h"

class World
{
public:
    Debug debug;

    World();

    virtual void get_world() = 0;


private:


};

#endif