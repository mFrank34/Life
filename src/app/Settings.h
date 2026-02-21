/*
 * File: Settings.h
 * Author: Michael Franks
 * Description: settings for application
 */

#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

enum class StorageType
{
    Sparse, Cache, Unordered
};

class Settings
{
public:
    // World configuration
    StorageType storage_type = StorageType::Sparse;

    // Simulation
    int tick_rate = 60;

    // Display
    bool show_grid = true;
    float zoom = 1.0f;
};

#endif // SETTINGS_H
