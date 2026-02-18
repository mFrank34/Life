/*
 * File: Simulation.h
 * Author: Michael Franks
 * Description: tick system for runtime, telling app what to do each tick
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "world/Manager.h"
#include "threading/Scheduler.h"
#include "world/World.h"

class Simulation
{
public:
    Simulation();
    ~Simulation() = default;

    void attach_scheduler(Scheduler& scheduler);
    void attach_world(World& world);

    void tick(float delta);

    void start();
    void pause();
    void set_speed(float speed);

    bool isRunning() const;

private:
    World* world = nullptr;
    Scheduler* scheduler = nullptr;

    Rules rules;
    Manager manager;

    bool running = true;
};

#endif
