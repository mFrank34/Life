#ifndef CONTROLLER_H
#define CONTROLLER_H
#pragma once
#include "../view/Interface.h"

enum class SimSpeed;
enum class CellColor;

class Controller
{
public:
    explicit Controller(World& world);

    // Simulation control
    void start();
    void pause();
    void restart();
    void generate();

    // Editor / UI actions
    void set_speed(SimSpeed speed);
    void set_color(CellColor color);

    SimSpeed speed() const;
    CellColor color() const;

    // Called by View
    void paint_at(double wx, double wy);

private:
    World& world;

    // SimSpeed speed_ = SimSpeed::X1;
    // CellColor color_ = CellColor::White;
    // bool running_ = false;
};

#endif //CONTROLLER_H
