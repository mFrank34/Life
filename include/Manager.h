#pragma once
#ifndef MANAGER_H
#define MANAGER_H

#include "Manager.h"
#include "Unordered.h"
#include "Life.h"

class Manager
{
public:
    Manager(Unordered *map, Life *rules);

    void update();

    ~Manager();

private:
    Unordered *map = nullptr; // hands over map
    Life *rules = nullptr; // rules of the game

};

#endif