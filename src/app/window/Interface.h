/*
 * File: Interface.h
 * Author: Michael Franks
 * Description: interface buttons and menu
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtkmm.h>

#include "View.h"
#include "app/Settings.h"
#include "app/simulation/Simulation.h"
#include "panels/GeneratePanel.h"
#include "panels/RulePanel.h"
#include "panels/SettingPanel.h"

enum class SimSpeed
{
    X1, X2, X4, X8
};

enum class CellColor
{
    Blue, Red, Green, White
};

class World;

namespace app::window
{
    class Interface : public Gtk::Box
    {
    public:
        Interface(View& view, Settings& settings, Simulation& simulation);

        // Signal emitted when user wants to change storage type
        sigc::signal<void(StorageType)> signal_storage_changed;

    private:
        // References (non-owning)
        View& view;
        Settings& settings;
        Simulation& simulation;

        // Layout
        Gtk::Overlay overlay;
        Gtk::Box left_panel{Gtk::Orientation::VERTICAL};
        Gtk::Box bottom_controls{Gtk::Orientation::HORIZONTAL};

        // Buttons
        Gtk::Button btn_blue, btn_red, btn_green, btn_white;
        Gtk::Button btn_speed;
        Gtk::Button btn_start, btn_pause, btn_step, btn_restart;
        Gtk::Button btn_generate, btn_rule_editor;
        Gtk::Button btn_settings;

        // State (UI-only)
        SimSpeed current_speed = SimSpeed::X1;
        CellColor current_color = CellColor::White;

        // Panels
        std::unique_ptr<GeneratePanel> generate_panel;
        std::unique_ptr<RulePanel> rule_panel;
        std::unique_ptr<SettingPanel> settings_panel;

        // Handlers
        void on_start() const;
        void on_pause() const;
        void on_step() const;
        void on_restart() const;
        void on_speed_cycle();
        void on_generate();
        void on_rule_editor();
        void on_settings();
    };
}

#endif
