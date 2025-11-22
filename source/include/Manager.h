#pragma once
#ifndef MANAGER_H
#define MANAGER_H

#include "Manager.h"
#include "Sparse.h"
#include "Rules.h"

class Manager
{
public:
    Manager( World& world,  Rules& rules);

    void update() ;

    void step();

    void render_step();

    ~Manager();

private:
    World& world;
    Rules& rules;
};

#endif