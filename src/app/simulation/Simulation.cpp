/*
 * File: Simulation.cpp
 * Author: Michael Franks
 * Description: body to simulation tick system and telling what to do... and so on
 */

#include "Simulation.h"
#include "app/Global.h"
#include "app/window/View.h"

Simulation::Simulation(Scheduler& scheduler, Global& global)
    : manager(global.rules, scheduler),
      generator(scheduler)
{
}

void Simulation::attach_world(World& world)
{
    this->world = &world;
    manager.attach_world(world);
    generator.attach_world(world);
    if (view) view->attach_world(world);
}

void Simulation::attach_view(View& view)
{
    this->view = &view;
}


void Simulation::tick(float delta)
{
    if (!this->isRunning() || !world)
        return;

    accumulator += delta;

    if (accumulator >= step_interval)
    {
        manager.update();
        // subtract rather than reset to avoid drift
        accumulator -= step_interval;
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

void Simulation::generate(GeneratorRequest request)
{
    pause();
    generator.GenerateRequest(request);
}

bool Simulation::isRunning() const
{
    return running;
}

void Simulation::set_world(World* new_world)
{
    pause();

    // Wait for any in-progress update to finish
    while (manager.is_updating())
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    world = new_world;
    attach_world(*world);
}
