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
    int radius = 0;
    int point_count = 0; // 0 = use default scaling

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

    // Main entry point to generate points/colors in the world
    void GenerateRequest(GeneratorRequest& request, std::function<void()> on_complete = nullptr);

private:
    World* world = nullptr;
    Scheduler& scheduler;
};

#endif // GENERATOR_H
