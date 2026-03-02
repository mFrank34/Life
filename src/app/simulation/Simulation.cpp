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
    if (!world) return;

    if (benchmarking)
    {
        auto now = std::chrono::high_resolution_clock::now();
        benchmark_elapsed = std::chrono::duration<float>(
            now - benchmark_start
        ).count();

        if (benchmark_elapsed >= benchmark_duration)
        {
            benchmarking = false;
            pause();

            Perf::Profiler::get().stop_recording();

            while (manager.is_updating())
                std::this_thread::sleep_for(std::chrono::milliseconds(1));

            Perf::Profiler::get().dump(benchmark_output);
            Perf::Profiler::get().set_record_tasks(true); // reset for next run
            Perf::Profiler::get().start_recording();

            world->clear_world();
            Logger::info("benchmark complete — saved to " + benchmark_output);

            return;
        }
    }

    if (!isRunning()) return;

    accumulator += delta;
    if (accumulator >= step_interval)
    {
        manager.update();
        accumulator -= step_interval;
        if (accumulator > step_interval)
            accumulator = 0.0f;
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

void Simulation::start_benchmark(GeneratorRequest& request, const std::string& output_path, bool record_tasks)
{
    benchmark_output = output_path;
    benchmark_elapsed = 0.0f;
    benchmarking = false;

    Perf::Profiler::get().clear();
    Perf::Profiler::get().set_record_tasks(record_tasks); // false for Unordered

    if (world) world->clear_world();

    generator.GenerateRequest(request, [this]()
    {
        benchmark_start = std::chrono::high_resolution_clock::now();
        benchmarking = true;
        start();
    });
}

bool Simulation::is_updating()
{
    return manager.is_updating();
}