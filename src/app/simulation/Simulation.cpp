#include "Simulation.h"

Simulation::Simulation(World& world)
    : world(world)
{
}

void Simulation::start()
{
    running = true;
}

void Simulation::stop()
{
    running = false;
}

bool Simulation::is_running() const
{
    return running;
}

void Simulation::tick(float delta)
{
    if (!running)
        return;

    world.tick(delta);
}
