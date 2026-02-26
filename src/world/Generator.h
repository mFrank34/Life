/*
 * File: Generator
 * Author: Michael Franks 
 * Description: Pattern Generator System
 */

#ifndef GENERATOR_H
#define GENERATOR_H
#include <string>

#include "World.h"
#include "threading/Scheduler.h"


/**
 * Request for generator
 */
struct GeneratorRequest
{
    std::string seed;
    int radius = 0;

    bool use_white = false;
    bool use_red = false;
    bool use_green = false;
    bool use_blue = false;
};


class Generator
{
public:
    Generator(Scheduler& scheduler);

    void attach_world(World& world);

private:
    World* world = nullptr;
    Scheduler& scheduler;
};


#endif // GENERATOR_H