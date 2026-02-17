/*
* File: Scheduler.cpp
 * Author: Michael Franks
 * Description: Fixed thread-safe Scheduler implementation
 */

#include "Scheduler.h"

Scheduler::Scheduler(size_t num_threads)
    : stop(false), active_tasks(0), completed_tasks(0)
{
    for (size_t i = 0; i < num_threads; ++i)
    {
        workers.emplace_back([this, i] { worker(i); });
    }
}

Scheduler::~Scheduler()
{
    {
        std::unique_lock<std::mutex> lock(queue_mtx);
        stop = true;
    }

    cv.notify_all(); // wake all workers to exit

    for (auto& worker : workers)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}

int Scheduler::get_active_tasks() const
{
    return active_tasks.load();
}

int Scheduler::get_completed_tasks() const
{
    return completed_tasks.load();
}

int Scheduler::get_pending_tasks() const
{
    std::unique_lock<std::mutex> lock(queue_mtx);
    return static_cast<int>(tasks.size());
}

void Scheduler::worker(int id)
{
    while (true)
    {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queue_mtx);
            cv.wait(lock, [this] { return stop || !tasks.empty(); });

            if (stop && tasks.empty())
            {
                break; // exit thread
            }

            task = std::move(tasks.front());
            tasks.pop();
        }

        if (task)
        {
            active_tasks.fetch_add(1, std::memory_order_relaxed);
            task();
            active_tasks.fetch_sub(1, std::memory_order_relaxed);
            completed_tasks.fetch_add(1, std::memory_order_relaxed);
        }
    }
}