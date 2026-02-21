/*
 * File: App.cpp
 * Author: Michael Franks
 * Description: Application Entry Point
 */

#include <gtkmm.h>
#include <iostream>

#include "app/window/Window.h"

int main(int argc, char* argv[])
{
    std::cout<< "Application Life Running" << std::endl;
    auto app = Gtk::Application::create("app.life.simulation");
    return app->make_window_and_run<Window>(argc, argv);
}