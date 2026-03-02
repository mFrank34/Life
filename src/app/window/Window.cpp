/*
 * File: Window.cpp
 * Author: Michael Franks
 * Description: Application Runtime for whole app
 */

#include "Window.h"
#include "app/Global.h"

#include "app/window/Interface.h"
#include "world/storage/Cache.h"


using namespace app::window;

Window::Window()
{
    set_title("Project Life");
    set_default_size(1200, 800);

    Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &Window::on_tick),
        16
    );

    Logger::info("Project Life Initialized");
    initialize();
}

void Window::initialize()
{
    // 1. Set up world first
    settings.set_world(std::make_unique<Cache>(32, 64));

    // 2. Create view before simulation since simulation needs it
    view = std::make_unique<View>(*settings.get_world(), simulation, settings);

    // 3. Attach world and view to simulation
    simulation.attach_world(*settings.get_world());
    simulation.attach_view(*view);
    simulation.start();

    // 4. Build UI
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