/*
 * File: Window.cpp
 * Author: Michael Franks
 * Created: 2026-02-17
 * Last Edited: 2026-02-14
 * Description:
 */

#include "Window.h"
#include "app/window/Interface.h"
#include "world/storage/Sparse.h"

using namespace app::window;

Window::Window()
{
    set_title("Project Life");
    set_default_size(800, 600);

    Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &Window::on_tick),
        16
    );

    initialize();
}

void Window::initialize()
{
    world = std::make_unique<Sparse>(32);

    view = std::make_unique<View>(*world);

    simulation.attach_world(*world);
    simulation.pause();

    interface = std::make_unique<app::window::Interface>(*world, *view,  settings, simulation);
    set_child(*interface);
}

bool Window::on_tick()
{
    if (!simulation.isRunning())
        return true;

    simulation.tick(0.016f);
    view->queue_draw();

    return true;
}