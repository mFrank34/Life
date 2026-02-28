/*
 * File: Profiler
 * Author: Michael Franks 
 * Description: Application profiler for finding out
 * memory and timing of update and render with threading
 */

#include "Perf.h"

namespace Perf
{
    Profiler& Profiler::get()
    {
        static Profiler instance;
        return instance;
    }

    void Profiler::begin_update()
    {
    }

    void Profiler::end_update()
    {
    }

    void Profiler::begin_render()
    {
    }

    void Profiler::end_render()
    {
    }

    void Profiler::record(int total_chunks, int populated_chunks, unsigned long memory_bytes)
    {
    }

    void Profiler::record_task(int thread_id, double duration_ms)
    {
    }

    void Profiler::dump(const std::string& path)
    {
    }
} // Profiler
