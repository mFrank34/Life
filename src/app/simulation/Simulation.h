/*
 * File: Simulation.h
 * Author: Michael Franks
 * Description: tick system for runtime, telling app what to do each tick
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "app/Global.h"
#include "world/Generator.h"
#include "world/Manager.h"
#include "world/World.h"

class Simulation
{
public:
    Simulation(Scheduler& scheduler, Global& settings);
    ~Simulation() = default;

    void attach_world(World& world);
    void tick(float delta);

    void start();
    void step();
    void pause();

    void clear();

    void set_speed(float generations_per_second);

    void generate(GeneratorRequest request);

    bool isRunning() const;

private:
    World* world = nullptr;
    Scheduler* scheduler = nullptr;

    Rules rules;
    Manager manager;
    Generator generator;

    bool running = true;

    float accumulator = 0.0f;
    float step_interval = 0.1f;
};

#endif