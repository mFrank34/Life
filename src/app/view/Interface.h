#ifndef INTERFACE_H
#define INTERFACE_H

#pragma once
#include <gtkmm.h>

#include "View.h"
#include "app/controller/Controller.h"

class World;
class Manager;
class Controller;

enum class SimSpeed
{
    X1 = 1,
    X2 = 2,
    X4 = 4,
    X8 = 8
};

enum class CellColor
{
    Blue,
    Red,
    Green,
    White
};

class Interface : public Gtk::Box
{
public:
    Interface(World& world, Manager& manager);

private:
    World& world;
    Manager& manager;

    // UI owner
    Gtk::Overlay overlay;
    View view;

    // Bottom controls
    Gtk::Box bottom_controls{Gtk::Orientation::HORIZONTAL};

    Gtk::Button btn_start;
    Gtk::Button btn_pause;
    Gtk::Button btn_restart;
    Gtk::Button btn_generate;
    Gtk::Button btn_rule_editor;
    Gtk::Button btn_import;
    Gtk::Button btn_export;
    Gtk::Button btn_settings;

    // Callbacks
    void on_start();
    void on_pause();
    void on_restart();
    void on_generate();
    void on_rule_editor();
    void on_import();
    void on_export();
    void on_settings();

    // Right panel (speed control)
    Gtk::Box right_panel{Gtk::Orientation::VERTICAL};

    Gtk::Button btn_speed_1;
    Gtk::Button btn_speed_2;
    Gtk::Button btn_speed_4;
    Gtk::Button btn_speed_8;

    void on_speed(SimSpeed speed);
    void update_speed_ui();

    // Left panel (colour selection)
    Gtk::Box left_panel{Gtk::Orientation::VERTICAL};

    Gtk::Button btn_blue{"Blue"};
    Gtk::Button btn_red{"Red"};
    Gtk::Button btn_green{"Green"};
    Gtk::Button btn_white{"White"};

    void on_color(CellColor color);
    void update_color_ui();
};

#endif
