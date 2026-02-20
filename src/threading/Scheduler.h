#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <atomic>

#include "utility/Logger.h"

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
            if (stop.load(std::memory_order_acquire))
                return;

            tasks.emplace(std::forward<F>(task));
        }
        cv.notify_one();
    }

    template <typename F>
    void enqueue_grouped(F&& task)
    {
        group_tasks.fetch_add(1, std::memory_order_relaxed);

        enqueue([this, task = std::forward<F>(task)]() mutable
        {
            task();

            if (group_tasks.fetch_sub(1, std::memory_order_acq_rel) == 1)
            {
                std::lock_guard<std::mutex> lock(group_mtx);
                group_cv.notify_one();
            }
        });
    }

    void wait_for_group();

    int get_active_tasks() const;
    int get_completed_tasks() const;
    int get_pending_tasks() const;

private:
    void worker(int id);

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    mutable std::mutex queue_mtx;
    std::condition_variable cv;

    std::atomic<bool> stop{false};

    std::atomic<int> active_tasks;
    std::atomic<int> completed_tasks;

    std::atomic<int> group_tasks{0};
    std::mutex group_mtx;
    std::condition_variable group_cv;
};

#endif // SCHEDULER_H