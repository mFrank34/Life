/*
 * File: Logger
 * Author: Michael Franks 
 * Description: application logger for debugging information
 */

#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <mutex>

class Logger
{
public:
    template <typename T>
    static void info(const T& msg)
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[INFO] " << msg << "\n";
    }

private:
    static std::mutex mtx;
};

#endif // LOGGER_H
