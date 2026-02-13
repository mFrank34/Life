#include <iostream>
#include <string>

// gtk
#include <gtkmm.h>

// custom class
#include "../world/Manager.h"
#include "rules/Rules.h"

// storage containers
#include "world/World.h"
#include "world/storage/Sparse.h"

// UI
#include "window/Interface.h"

#include "Settings.h"
#include "app/simulation/Simulation.h"

int main(int argc, char* argv[])
{
    auto app = Gtk::Application::create();

    Settings settings;
    Simulation sim(settings);

    Gtk::Window main_window;
    main_window.set_title("Simulation");

    Glib::signal_timeout().connect(
        sigc::mem_fun(sim, &Simulation::tick),
        1000 / settings.tick_rate
    );

    return app->run(interface());
}
