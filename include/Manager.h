#pragma once
#ifndef MANAGER_H
#define MANAGER_H

#include "Manager.h"
#include "Map.h"
#include "Life.h"

class Manager
{
public:
    Manager(Map *Map);

    void update();

    ~Manager();

private:
    Map *map = nullptr; // hands over map
    Life rules; // rules of the game

};

#endif