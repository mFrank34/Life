#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm/window.h>
#include <memory>

#include "app/window/Interface.h"
#include "world/World.h"
#include "app/Settings.h"

class Window : public Gtk::Window
{
public:
    Window();

private:
    void initialize();

    Settings settings;
    std::unique_ptr<World> world;
    std::unique_ptr<app::window::Interface> interface;
};

#endif
