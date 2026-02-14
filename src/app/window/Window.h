#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm/window.h>
#include <memory>

#include "app/window/Interface.h"
#include "world/World.h"
#include "app/Settings.h"
#include "app/simulation/Simulation.h"

class Window : public Gtk::Window
{
public:
    Window();

private:
    void initialize();
    bool on_tick();

    Settings settings;
    Simulation simulation = Simulation();

    // application resources for life
    std::unique_ptr<World> world;
    std::unique_ptr<View> view;
    std::unique_ptr<app::window::Interface> interface;
};

#endif
