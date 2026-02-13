// Simulation.h
#include "Simulation.h"
#include <gtkmm.h>
#include <chrono>
#include <iostream>

Simulation::Simulation(Settings& settings)
    : settings(settings),
      last(std::chrono::steady_clock::now())
{
}


bool Simulation::tick()
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> delta = now - last;
    last = now;

    double dt = delta.count();
    std::cout << "dt = " << dt << std::endl;

    return true;
}
