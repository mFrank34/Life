/*
* File: Settings.h
 * Author: Michael Franks
 * Description: settings for application
 */

#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include <memory>
#include "rules/Rules.h"
#include "world/structure/Cell.h"
#include "world/World.h"

class Global
{
public:
    // World
    std::unique_ptr<World> world;

    void set_world(std::unique_ptr<World> new_world)
    {
        world = std::move(new_world);
    }

    World* get_world() const
    {
        return world.get();
    }

    // Simulation
    int tick_rate = 60;
    Rules rules;

    // Display
    bool show_grid = true;
    bool debug = false;
    float zoom = 1.0f;

    CellType colour = CellType::White;
};

#endif // SETTINGS_H