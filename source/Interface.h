#ifndef INTERFACE_H
#define INTERFACE_H

#pragma once
#include <gtkmm.h>

class World;
class Manager;
class View;

class Interface : public Gtk::Box
{
public:
    Interface(World& world, Manager& manager);

private:
    World& world;
    Manager& manager;

    Gtk::Stack stack;

    Gtk::Widget* create_start_page();
    Gtk::Widget* create_main_ui();
};

#endif // INTERFACE_H
