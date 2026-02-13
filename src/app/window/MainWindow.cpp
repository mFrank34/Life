//
// Created by frank on 13/02/2026.
//

#include "MainWindow.h"

// app/MainWindow.cpp
MainWindow::MainWindow()
    : simulation(settings)
{
    set_title("Simulation");

    sim_timer = Glib::signal_timeout().connect(
        sigc::mem_fun(simulation, &Simulation::tick),
        1000 / settings.tick_rate
    );
}
