/*
 * File: App.cpp
 * Author: Michael Franks
 * Created: 2026-02-17
 * Last Edited: 2026-02-14
 * Description:
 */

// App.cpp

#include <gtkmm.h>
#include <iostream>

#include "app/window/Window.h"

int main(int argc, char* argv[])
{
    std::cout<< "Application Life Running" << std::endl;
    auto app = Gtk::Application::create("app.life.simulation");
    return app->make_window_and_run<Window>(argc, argv);
}