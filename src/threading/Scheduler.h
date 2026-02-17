/*
* File: Scheduler.h
 * Author: Michael Franks
 * Description: Thread-safe task scheduler
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <atomic>

class Scheduler
{
public:
    Scheduler(size_t num_threads);
    ~Scheduler();

    // Add a new task to the scheduler
    template <typename F>
    void enqueue(F&& task)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mtx);
            tasks.emplace(std::forward<F>(task));
        }
        cv.notify_one(); // wake one worker
    }

    int get_active_tasks() const;
    int get_completed_tasks() const;
    int get_pending_tasks() const;

private:
    void worker(int id);

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    mutable std::mutex queue_mtx; // mutable so const functions can lock
    std::condition_variable cv;

    bool stop;

    std::atomic<int> active_tasks;
    std::atomic<int> completed_tasks;
};

#endif // SCHEDULER_H
