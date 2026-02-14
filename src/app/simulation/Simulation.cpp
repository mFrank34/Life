#include "Simulation.h"

Simulation::Simulation()
    : manager(rules)
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
    manager.update();
}

void Simulation::start()
{
    running = true;
}

void Simulation::pause()
{
    running = false;
}

void Simulation::set_speed(float speed)
{
    // TODO: Create a speed changer that changed the speed which simulation happens with
}

bool Simulation::isRunning() const
{
    return running;
}
