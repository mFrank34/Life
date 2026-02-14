// App.cpp

#include <gtkmm.h>
#include <iostream>

#include "app/window/Window.h"

int main(int argc, char* argv[])
{
    std::cout<< "Application Life Running" << std::endl;

    auto app = Gtk::Application::create("com.example.simulation");
    return app->make_window_and_run<Window>(argc, argv);
}
