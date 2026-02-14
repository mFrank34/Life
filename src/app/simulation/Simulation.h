#ifndef SIMULATION_H
#define SIMULATION_H

#include "world/Manager.h"
#include "world/World.h"

class Simulation
{
public:
    Simulation();
    ~Simulation() = default;

    void attach_world(World& world);
    void tick(float delta);

    void start();
    void pause();
    void set_speed(float speed);

    bool isRunning() const;

private:
    World* world = nullptr;
    Rules rules;
    Manager manager;

    bool running = true;
};

#endif
