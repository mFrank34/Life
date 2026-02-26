/*
 * File: Generator
 * Author: Michael Franks 
 * Description:
 */

#include "Generator.h"
#include "app/simulation/Simulation.h"

Generator::Generator(Scheduler& scheduler)
    : scheduler(scheduler)
{
}

void Generator::attach_world(World& world)
{
    this->world = &world;
}