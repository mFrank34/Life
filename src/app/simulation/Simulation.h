/*
 * File: Simulation.h
 * Author: Michael Franks
 * Description: tick system for runtime, telling app what to do each tick
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "app/Global.h"
#include "world/Generator.h"
#include "world/Manager.h"
#include "world/World.h"

class View;

class Simulation
{
public:
    Simulation(Scheduler& scheduler, Global& global);
    ~Simulation() = default;

    void attach_world(World& world);
    void attach_view(View& view);

    void tick(float delta);

    void start();
    void step();
    void pause();

    void clear();

    void set_speed(float generations_per_second);

    void generate(GeneratorRequest request);

    bool isRunning() const;

    void set_world(World* world);

    void start_benchmark(
        GeneratorRequest& request,
        const std::string& output_path = "profile.json"
    );

    bool is_benchmarking() const { return benchmarking; }

    bool is_updating();

private:
    World* world = nullptr;
    Scheduler* scheduler = nullptr;
    View* view = nullptr;

    Rules rules;
    Manager manager;
    Generator generator;

    bool running = true;

    float accumulator = 0.0f;
    float step_interval = 0.1f;

    // Benchmark
    bool benchmarking = false;
    float benchmark_elapsed = 0.0f;
    float benchmark_duration = 120.0f; // 2 minutes
    std::string benchmark_output = "profile.json";
};

#endif
