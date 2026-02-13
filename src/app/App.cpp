#include <iostream>
#include <string>

// gtk
#include <gtkmm.h>

// custom class
#include "../world/Manager.h"
#include "rules/Rules.h"
#include "world/Debug.h"

// storage containers
#include "world/World.h"
#include "world/storage/Sparse.h"

// UI
#include "view/Interface.h"

Debug global_debug;

/* --- Test pattern --- */

void blinker(World& world)
{
    for (int y = 1; y <= 3; ++y)
        world.get_cell(1, y).set_type('w');
}

/* --- Headless--- */

int run_headless(int steps)
{
    constexpr int size = 3;
    Sparse world(size);
    Rules rules;
    Manager life(world, rules);

    global_debug.register_world(0);

    std::cout << "Starting Pos:\n";
    blinker(world);
    global_debug.all_chunks();

    for (int index = 0; index < steps; index++)
    {
        std::cout << "Step: " << index << "\n";
        life.update();
        world.unload();
        global_debug.all_chunks();
    }

    return 0;
}

/* --- GTK Interface --- */

int run_interface()
{
    auto app = Gtk::Application::create("life.Simulation");

    constexpr int size = 16;
    Sparse world(size);
    Rules rules;
    Manager life(world, rules);

    blinker(world);

    app->signal_activate().connect([&]()
    {
        auto window = new Gtk::Window();
        window->set_title("Conway's Game of Life");
        window->set_default_size(800, 600);

        auto ui = Gtk::make_managed<Interface>(world);
        window->set_child(*ui);

        app->add_window(*window);
        window->present();
    });

    return app->run();
}

/* --- Main --- */

int main(int argc, char** argv)
{
    bool use_ui = true;
    int steps = 3;


    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "--interface")
        {
            use_ui = true;
        }
        else if (arg == "--steps" && i + 1 < argc)
        {
            steps = std::stoi(argv[++i]);
        }
    }

    // UI mode
    if (use_ui)
        return run_interface();

    // Headless mode
    return run_headless(steps);
}
