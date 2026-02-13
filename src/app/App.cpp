// gtk
#include <gtkmm.h>
#include "app/Settings.h"
#include "app/simulation/Simulation.h"
#include "app/window/MainWindow.h"

int main(int argc, char* argv[])
{
    auto app = Gtk::Application::create("com.example.simulation");

    MainWindow window;
    window.present(); // <-- show window

    return app->run(); // <-- no arguments
}
