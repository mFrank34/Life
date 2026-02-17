/*
 * File: Scheduler.cpp
 * Author: Michael Franks
 * Created: 2026-02-17
 * Last Edited: 2026-02-17
 * Description:
 */

// Scheduler.cpp

#include "Scheduler.h"

Scheduler::Scheduler(size_t num_threads)
    : stop(false)
{
}

Scheduler::~Scheduler()
{
}

int Scheduler::get_active_tasks() const
{
}

int Scheduler::get_completed_tasks() const
{
}

int Scheduler::get_pending_tasks() const
{
}

void Scheduler::worker(int id)
{
}