#ifndef SIMULATION_H
#define SIMULATION_H

#include "world/World.h"

class Simulation
{
public:
    explicit Simulation(World& world);

    void start();
    void stop();
    void tick(float delta);

    bool is_running() const;

private:
    World& world;
    bool running = false;
};

#endif
