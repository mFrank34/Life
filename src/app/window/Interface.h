#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtkmm.h>
#include <chrono>
#include "View.h"
#include "world/World.h"
#include "app/Settings.h"

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

class World; // Forward declaration


namespace app::window
{
    class Interface : public Gtk::Box
    {
    public:
        Interface(World& world, Settings& settings);

        void start_simulation();
        void pause_simulation();
        void set_speed(SimSpeed speed);

        // Signal emitted when user wants to change storage type
        sigc::signal<void(StorageType)> signal_storage_changed;

    private:
        World& world;
        Settings& settings;
        View view;

        // Overlay structure
        Gtk::Overlay overlay;

        // Left panel - color selection
        Gtk::Box left_panel{Gtk::Orientation::VERTICAL};
        Gtk::Button btn_blue;
        Gtk::Button btn_red;
        Gtk::Button btn_green;
        Gtk::Button btn_white;

        // Right panel - speed controls
        Gtk::Box right_panel{Gtk::Orientation::VERTICAL};
        Gtk::Button btn_speed_1;
        Gtk::Button btn_speed_2;
        Gtk::Button btn_speed_4;
        Gtk::Button btn_speed_8;

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

        // Simulation
        bool on_tick();
        sigc::connection sim_timer;
        bool is_running = false;
        SimSpeed current_speed = SimSpeed::X1;
        CellColor current_color = CellColor::Blue;
        std::chrono::steady_clock::time_point last_tick;

        int get_interval_ms() const;

        // Event handlers
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

        // UI updates
        void update_speed_ui();
        void update_color_ui();

        // Settings dialog
        void show_settings_dialog();
    };
};


#endif // INTERFACE_H
