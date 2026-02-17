/*
 * File: Scheduler.h
 * Author: Michael Franks
 * Description:
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class Scheduler
{
public:
    Scheduler(size_t num_threads);
    ~Scheduler();

    template <typename F>
    void enqueue(F&& task)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mtx);
            tasks.emplace(std::forward<F>(task));
        }
        cv.notify_all();
    }

    int get_active_tasks() const;
    int get_completed_tasks() const;
    int get_pending_tasks() const;

private:
    void worker(int id);

    // list of workers
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    // function mtx size
    std::mutex queue_mtx;
    std::condition_variable cv;

    // state
    bool stop;

    // task completion
    std::atomic<int> active_tasks;
    std::atomic<int> completed_tasks;
};

#endif // SCHEDULER_H