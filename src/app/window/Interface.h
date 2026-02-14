#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtkmm.h>

#include "View.h"
#include "app/Settings.h"
#include "app/simulation/Simulation.h"

enum class SimSpeed
{
    X1,
    X2,
    X4,
    X8
};

enum class CellColor
{
    Blue,
    Red,
    Green,
    White
};

class World;

namespace app::window
{
    class Interface : public Gtk::Box
    {
    public:
        Interface(World& world, Settings& settings, Simulation& simulation);

        // Signal emitted when user wants to change storage type
        sigc::signal<void(StorageType)> signal_storage_changed;

    private:
        // References (non-owning)
        World& world;
        Settings& settings;
        Simulation& simulation;

        // View
        View view;

        // Layout
        Gtk::Overlay overlay;

        Gtk::Box left_panel{Gtk::Orientation::VERTICAL};
        Gtk::Box right_panel{Gtk::Orientation::VERTICAL};
        Gtk::Box bottom_controls{Gtk::Orientation::HORIZONTAL};

        // Buttons
        Gtk::Button btn_blue, btn_red, btn_green, btn_white;
        Gtk::Button btn_speed_1, btn_speed_2, btn_speed_4, btn_speed_8;
        Gtk::Button btn_start, btn_pause, btn_restart;
        Gtk::Button btn_generate, btn_rule_editor;
        Gtk::Button btn_import, btn_export, btn_settings;

        // State (UI-only)
        SimSpeed current_speed = SimSpeed::X1;
        CellColor current_color = CellColor::Blue;

        // Handlers
        void on_start();
        void on_pause();
        void on_restart();
        void on_generate();
        void on_rule_editor();
        void on_import();
        void on_export();
        void on_settings();
        void on_speed(SimSpeed speed);
        void on_color(CellColor color);

        // UI helpers
        void update_speed_ui();
        void update_color_ui();
        void show_settings_dialog();
    };
}

#endif
