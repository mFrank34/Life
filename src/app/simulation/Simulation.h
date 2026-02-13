// Simulation.h

#pragma once
#ifndef SIMULATION_H
#define SIMULATION_H

#include <chrono>

class Settings;

class Simulation
{
public:
    Simulation(Settings& settings);

    bool tick();

private:
    std::chrono::steady_clock::time_point last;
    Settings& settings;
};


#endif // SIMULATION_H
