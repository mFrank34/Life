// Simulation.h
#include "Simulation.h"
#include <gtkmm.h>
#include <chrono>

Simulation::Simulation(Settings& settings)
    : settings(settings)
{
}

bool Simulation::tick()
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> delta = now - last;
    last = now;

    double dt = delta.count(); // seconds

    // simulation step using dt
    g_print("dt = %f\n", dt);

    return true;
};
