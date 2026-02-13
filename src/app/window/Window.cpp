// Window.cpp

#include "Window.h"
#include <glibmm/main.h>

Window::Window()
    : simulation(settings)
{
    set_title("Simulation");
    set_default_size(800, 600);


    // Connect AFTER window exists
    sim_timer = Glib::signal_timeout().connect(
        sigc::mem_fun(simulation, &Simulation::tick),
        1000 / settings.tick_rate
    );
}
