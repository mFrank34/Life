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

    simulation.attach_world(*world);
    simulation.pause();

    interface = std::make_unique<app::window::Interface>(*world, settings, simulation);
    set_child(*interface);
}

bool Window::on_tick()
{
    if (!simulation.isRunning())
        return true;

    simulation.tick(0.016f);
    return true;
}
