/*
 * File: Window.cpp
 * Author: Michael Franks
 * Description: Application Runtime for whole app
 */

#include "Window.h"
#include "app/Settings.h"

#include "app/window/Interface.h"
#include "world/storage/Cache.h"
#include "world/storage/Sparse.h"
#include "world/storage/Unordered.h"

using namespace app::window;

Window::Window()
{
    set_title("Project Life");
    set_default_size(1200, 800);

    Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &Window::on_tick),
        16
    );

    initialize();
}

void Window::initialize()
{
    world = std::make_unique<Sparse>(32);
    view = std::make_unique<View>(*world, simulation, settings);

    simulation.attach_world(*world);
    simulation.start();

    interface = std::make_unique<app::window::Interface>(*view, settings, simulation);
    set_child(*interface);
}

bool Window::on_tick()
{
    view->queue_draw();

    if (!simulation.isRunning())
        return true;

    simulation.tick(0.016f);
    return true;
}