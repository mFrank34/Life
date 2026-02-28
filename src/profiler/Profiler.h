/*
 * File: Profiler
 * Author: Michael Franks 
 * Description: a system to track application memory and update timing of application
 */

#pragma once
#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>
#include <mutex>
#include <string>
#include <vector>

namespace Perf
{
    struct Sample
    {
        double update_ms;
        double render_ms;
        double total_ms;

        int total_chunks;
        unsigned long memory_bytes;
        long long timestamp_ms;
    };

    struct TaskSample
    {
        int thread_id;
        double duration_ms;
        long long timestamp_ms;
    };

    class Profiler
    {
    public:
        static Profiler& get();

        Profiler(const Profiler&) = delete;
        Profiler& operator=(const Profiler&) = delete;

        void begin_update();
        void end_update();

        void begin_render();
        void end_render();

        void record(int total_chunks, int populated_chunks, unsigned long memory_bytes);
        void record_task(int thread_id, double duration_ms);
        void dump(const std::string& path);

    private:
        Profiler() = default;

        std::vector<Sample> samples;
        std::vector<TaskSample> task_samples;
        mutable std::mutex task_mtx;

        std::chrono::high_resolution_clock::time_point update_start;
        std::chrono::high_resolution_clock::time_point render_start;

        double last_update_ms = 0.0;
        double last_render_ms = 0.0;

        std::chrono::high_resolution_clock::time_point session_start
            = std::chrono::high_resolution_clock::now();
    };
} // Profiler

#endif // PROFILER_H
