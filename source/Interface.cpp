#include "Interface.h"
#include "View.h"
#include "World.h"
#include "Manager.h"

Interface::Interface(World& world, Manager& manager)
    : Gtk::Box(Gtk::Orientation::VERTICAL),
      world(world),
      manager(manager),
      view(world)
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

    left_panel.append(btn_blue);
    left_panel.append(btn_red);
    left_panel.append(btn_green);
    left_panel.append(btn_white);

    btn_blue.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_color), CellColor::Blue));
    btn_red.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_color), CellColor::Red));
    btn_green.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_color), CellColor::Green));
    btn_white.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_color), CellColor::White));

    // --- RIGHT PANEL (speed controls) ---
    right_panel.set_halign(Gtk::Align::END);
    right_panel.set_valign(Gtk::Align::START);
    right_panel.set_margin(8);
    right_panel.set_spacing(6);
    overlay.add_overlay(right_panel);

    btn_speed_1.set_label("x1");
    btn_speed_2.set_label("x2");
    btn_speed_4.set_label("x4");
    btn_speed_8.set_label("x8");

    btn_speed_1.add_css_class("speed-btn");
    btn_speed_2.add_css_class("speed-btn");
    btn_speed_4.add_css_class("speed-btn");
    btn_speed_8.add_css_class("speed-btn");

    right_panel.append(btn_speed_1);
    right_panel.append(btn_speed_2);
    right_panel.append(btn_speed_4);
    right_panel.append(btn_speed_8);

    btn_speed_1.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_speed), SimSpeed::X1));
    btn_speed_2.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_speed), SimSpeed::X2));
    btn_speed_4.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_speed), SimSpeed::X4));
    btn_speed_8.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_speed), SimSpeed::X8));

    // --- BOTTOM CONTROLS ---
    bottom_controls.set_margin(8);
    bottom_controls.set_spacing(12);
    bottom_controls.set_halign(Gtk::Align::CENTER);
    bottom_controls.set_valign(Gtk::Align::END);
    overlay.add_overlay(bottom_controls);

    btn_start.set_label("Start");
    btn_pause.set_label("Pause");
    btn_restart.set_label("Restart");
    btn_generate.set_label("Generate");
    btn_rule_editor.set_label("Rule Editor");
    btn_import.set_label("Import");
    btn_export.set_label("Export");
    btn_settings.set_label("Settings");

    bottom_controls.append(btn_start);
    bottom_controls.append(btn_pause);
    bottom_controls.append(btn_restart);
    bottom_controls.append(btn_generate);
    bottom_controls.append(btn_rule_editor);
    bottom_controls.append(btn_import);
    bottom_controls.append(btn_export);
    bottom_controls.append(btn_settings);

    btn_start.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_start));
    btn_pause.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_pause));
    btn_restart.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_restart));
    btn_generate.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_generate));
    btn_rule_editor.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_rule_editor));
    btn_import.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_import));
    btn_export.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_export));
    btn_settings.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_settings));
}


// TODO Build application interactions for application Section Pretty please michael! thanks your the best

void Interface::on_start()
{
}

void Interface::on_pause()
{
}

void Interface::on_restart()
{
}

void Interface::on_generate()
{
}

void Interface::on_rule_editor()
{
}

void Interface::on_import()
{
}

void Interface::on_export()
{
}

void Interface::on_settings()
{
}

void Interface::on_speed(SimSpeed speed)
{
}

void Interface::update_speed_ui()
{
}

void Interface::on_color(CellColor color)
{
}

void Interface::update_color_ui()
{
}
