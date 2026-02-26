/*
* File: Generator.h
 * Author: Michael Franks
 * Description: Pattern Generator System
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>
#include <vector>
#include "World.h"
#include "threading/Scheduler.h"

// Simple position struct
struct Location
{
    int x, y;
};

// Request for generator
struct GeneratorRequest
{
    std::string seed;
    int radius = 0; // optional radius; default will be half world size

    bool use_white = false;
    bool use_red   = false;
    bool use_green = false;
    bool use_blue  = false;
};

class Generator
{
public:
    Generator(Scheduler& scheduler);

    void attach_world(World& world);

    // Main entry point to generate points/colors in the world
    void GenerateRequest(GeneratorRequest& request);

private:
    World* world = nullptr;
    Scheduler& scheduler;

    // Note: scatterInRadius is implemented as a static free function in cpp
};

#endif // GENERATOR_H