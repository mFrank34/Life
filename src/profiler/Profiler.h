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
        /**
         * creates on instance of profiler
         * @return returns the only instance of profiler
         */
        static Profiler& get();

        /**
         * keeps only one instance
         */
        Profiler(const Profiler&) = delete;

        /**
         * copes and move data between profilers
         * @return return new Profiler
         */
        Profiler& operator=(const Profiler&) = delete;

        /**
         * Create new entry in samples
         */
        void begin_update();

        /**
         * ends that new entry in samples
         */
        void end_update();

        /**
         * create new entry for render
         */
        void begin_render();

        /**
         * ends the new entry for render
         */
        void end_render();

        /**
         * records the data for application usage
         * @param total_chunks current amount of chunks in system
         * @param memory_bytes memory data and usage
         */
        void record(int total_chunks, unsigned long memory_bytes);


        /**
         * collects data from a thread
         * @param thread_id name of thread
         * @param duration_ms time it takes for a thread to work
         */
        void record_task(int thread_id, double duration_ms);

        /**
         * dump data to a json for analysis
         * @param path location where that needs to be stored
         */
        void dump(const std::string& path);

    private:
        Profiler() = default;

        std::vector<Sample> samples;
        std::vector<TaskSample> task_samples;

        mutable std::mutex sample_mtx;
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
