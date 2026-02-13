// MainWindows.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm/window.h>
#include "simulation/Simulation.h"
#include "app/Settings.h"

class MainWindow : public Gtk::Window
{
public:
    MainWindow();

private:
    Settings settings;
    Simulation simulation;

    sigc::connection sim_timer;
};

#endif // MAINWINDOW_H
