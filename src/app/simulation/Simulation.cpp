/*
 * File: Simulation.cpp
 * Author: Michael Franks
 * Description: body to simulation tick system and telling what to do... and so on
 */

#include "Simulation.h"

Simulation::Simulation(Scheduler& scheduler)
    : manager(rules, scheduler)
{
}

void Simulation::attach_world(World& world)
{
    this->world = &world;
    manager.attach_world(world);
}


void Simulation::tick(float delta)
{
    if (!this->isRunning() || !world)
        return;

    accumulator += delta;

    if (accumulator >= step_interval)
    {
        manager.update();
        accumulator -= step_interval; // subtract rather than reset to avoid drift
    }
}

void Simulation::start()
{
    running = true;
}

void Simulation::step()
{
    if (!running)
        manager.update();
}

void Simulation::pause()
{
    running = false;
}

void Simulation::clear()
{
    world->clear_world();
}

void Simulation::set_speed(float generations_per_second)
{
    step_interval = 1.0f / generations_per_second;
}

bool Simulation::isRunning() const
{
    return running;
}