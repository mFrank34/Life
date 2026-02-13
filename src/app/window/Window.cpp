#include "Window.h"
#include "app/window/Interface.h"
#include "world/storage/Sparse.h"

using namespace app::window;

Window::Window()
{
    set_title("Simulation");
    set_default_size(800, 600);

    initialize();
}

void Window::initialize()
{
    if (interface)
    {
        interface->pause_simulation();
    }

    world = std::make_unique<Sparse>(32);

    interface = std::make_unique<app::window::Interface>(*world, settings);

    set_child(*interface);
}
