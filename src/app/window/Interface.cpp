/*
 * File: Interface.cpp
 * Author: Michael Franks
 * Description: interface buttons
 */

#include "app/window/Interface.h"
#include "utility/Logger.h"

namespace app::window
{
    Interface::Interface(View& view, Settings& settings, Simulation& simulation)
        : Gtk::Box(Gtk::Orientation::VERTICAL)
          , settings(settings)
          , simulation(simulation)
          , view(view)
    {
        set_hexpand(true);
        set_vexpand(true);

        // --- OVERLAY ---
        overlay.set_hexpand(true);
        overlay.set_vexpand(true);
        append(overlay);

        // --- WORLD VIEW ---
        overlay.set_child(view);

        // --- LEFT PANEL (colour selection) ---
        left_panel.set_halign(Gtk::Align::START);
        left_panel.set_valign(Gtk::Align::START);
        left_panel.set_margin(8);
        left_panel.set_spacing(6);
        overlay.add_overlay(left_panel);

        btn_blue.add_css_class("color-btn");
        btn_red.add_css_class("color-btn");
        btn_green.add_css_class("color-btn");
        btn_white.add_css_class("color-btn");

        btn_blue.set_label("Blue");
        btn_red.set_label("Red");
        btn_green.set_label("Green");
        btn_white.set_label("White");

        left_panel.append(btn_blue);
        left_panel.append(btn_red);
        left_panel.append(btn_green);
        left_panel.append(btn_white);

        // btn_blue.signal_clicked().connect(
        //     sigc::bind(sigc::mem_fun(*this, &Interface::on_color), CellColor::Blue));
        // btn_red.signal_clicked().connect(
        //     sigc::bind(sigc::mem_fun(*this, &Interface::on_color), CellColor::Red));
        // btn_green.signal_clicked().connect(
        //     sigc::bind(sigc::mem_fun(*this, &Interface::on_color), CellColor::Green));
        // btn_white.signal_clicked().connect(
        //     sigc::bind(sigc::mem_fun(*this, &Interface::on_color), CellColor::White));

        // --- BOTTOM CONTROLS ---
        bottom_controls.set_margin(8);
        bottom_controls.set_spacing(12);
        bottom_controls.set_halign(Gtk::Align::CENTER);
        bottom_controls.set_valign(Gtk::Align::END);
        overlay.add_overlay(bottom_controls);

        btn_start.set_label("Start");
        btn_pause.set_label("Pause");
        btn_step.set_label("Step");
        btn_speed.set_label("Speed: x1");
        btn_restart.set_label("Restart");
        btn_generate.set_label("Generate");
        btn_rule_editor.set_label("Rule Editor");
        btn_settings.set_label("Settings");

        bottom_controls.append(btn_start);
        bottom_controls.append(btn_step);
        bottom_controls.append(btn_pause);
        bottom_controls.append(btn_speed);
        bottom_controls.append(btn_restart);
        bottom_controls.append(btn_generate);
        bottom_controls.append(btn_rule_editor);
        bottom_controls.append(btn_settings);

        btn_start.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_start));
        btn_pause.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_pause));
        btn_step.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_step));
        btn_speed.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_speed_cycle));
        btn_restart.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_restart));
        btn_generate.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_generate));
        btn_rule_editor.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_rule_editor));
        btn_settings.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_settings));
    }

    // ===== Button handlers =====

    void Interface::on_start() const
    {
        simulation.start();
    }

    void Interface::on_pause() const
    {
        simulation.pause();
    }

    void Interface::on_step() const
    {
        simulation.step();
    }

    void Interface::on_restart() const
    {
        simulation.clear();
        view.queue_draw();
    }

    void Interface::on_speed_cycle()
    {
        switch (current_speed)
        {
        case SimSpeed::X1:
            current_speed = SimSpeed::X2;
            simulation.set_speed(4.0f);
            btn_speed.set_label("Speed: x2");
            break;

        case SimSpeed::X2:
            current_speed = SimSpeed::X4;
            simulation.set_speed(8.0f);
            btn_speed.set_label("Speed: x4");
            break;

        case SimSpeed::X4:
            current_speed = SimSpeed::X8;
            simulation.set_speed(16.0f);
            btn_speed.set_label("Speed: x8");
            break;

        case SimSpeed::X8:
            current_speed = SimSpeed::X1;
            simulation.set_speed(2.0f);
            btn_speed.set_label("Speed: x1");
            break;
        }
    }

    void Interface::on_generate()
    {
    }

    void Interface::on_rule_editor()
    {
    }

    void Interface::on_settings()
    {
    }


    // ===== UI helpers =====

    void Interface::update_speed_ui()
    {
    }

    void Interface::update_color_ui()
    {
        btn_blue.remove_css_class("active");
        btn_red.remove_css_class("active");
        btn_green.remove_css_class("active");
        btn_white.remove_css_class("active");

        switch (current_color)
        {
        case CellColor::Blue: btn_blue.add_css_class("active");
            break;
        case CellColor::Red: btn_red.add_css_class("active");
            break;
        case CellColor::Green: btn_green.add_css_class("active");
            break;
        case CellColor::White: btn_white.add_css_class("active");
            break;
        }
    }

    void Interface::show_settings_dialog()
    {
    }
}
