// Windows.h

#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm/window.h>
#include "app/simulation/Simulation.h"
#include "app/Settings.h"

class Window : public Gtk::Window
{
public:
    Window();

private:
    Settings settings;
    Simulation simulation;
    Interface interface;

    sigc::connection sim_timer;
};

#endif // WINDOW_H
