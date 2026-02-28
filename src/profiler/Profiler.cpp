/*
 * File: Profiler.cpp
 * Author: Michael Franks
 * Description: Application profiler for finding out
 * memory and timing of update and render with threading
 */

#include "Profiler.h"
#include <fstream>
#include <json.hpp>

namespace Perf
{
    Profiler& Profiler::get()
    {
        static Profiler instance;
        return instance;
    }

    void Profiler::begin_update()
    {
        update_start = std::chrono::high_resolution_clock::now();
    }

    void Profiler::end_update()
    {
        auto end = std::chrono::high_resolution_clock::now();
        last_update_ms = std::chrono::duration<double, std::milli>(end - update_start).count();
    }

    void Profiler::begin_render()
    {
        render_start = std::chrono::high_resolution_clock::now();
    }

    void Profiler::end_render()
    {
        auto end = std::chrono::high_resolution_clock::now();
        last_render_ms = std::chrono::duration<double, std::milli>(end - render_start).count();
    }

    void Profiler::record(int total_chunks, unsigned long memory_bytes)
    {
        auto now = std::chrono::high_resolution_clock::now();
        long long ts = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - session_start
        ).count();

        Sample sample = {
            last_update_ms,
            last_render_ms,
            last_update_ms + last_render_ms,
            total_chunks,
            memory_bytes,
            ts
        };

        std::lock_guard<std::mutex> lock(sample_mtx);
        samples.push_back(sample);
    }

    void Profiler::record_task(int thread_id, double duration_ms)
    {
        auto now = std::chrono::high_resolution_clock::now();
        long long ts = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - session_start
        ).count();

        TaskSample task = {
            thread_id,
            duration_ms,
            ts
        };

        std::lock_guard<std::mutex> lock(task_mtx);
        task_samples.push_back(task);
    }

    void Profiler::dump(const std::string& path)
    {
        nlohmann::json file;
        file["samples"] = nlohmann::json::array();
        file["tasks"] = nlohmann::json::array();

        {
            std::lock_guard<std::mutex> lock(sample_mtx);
            for (const auto& s : samples)
            {
                file["samples"].push_back({
                    {"update_ms", s.update_ms},
                    {"render_ms", s.render_ms},
                    {"total_ms", s.total_ms},
                    {"total_chunks", s.total_chunks},
                    {"memory_bytes", s.memory_bytes},
                    {"timestamp_ms", s.timestamp_ms}
                });
            }
        }

        {
            std::lock_guard<std::mutex> lock(task_mtx);
            for (const auto& t : task_samples)
            {
                file["tasks"].push_back({
                    {"thread_id", t.thread_id},
                    {"duration_ms", t.duration_ms},
                    {"timestamp_ms", t.timestamp_ms}
                });
            }
        }

        std::ofstream out(path);
        out << file.dump(2);
    }
} // namespace Perf
