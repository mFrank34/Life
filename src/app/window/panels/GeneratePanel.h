/*
 * File: Generate
 * Author: Michael Franks 
 * Description: view for generate section of application
 */

#ifndef GENERATE_PANEL_H
#define GENERATE_PANEL_H

#include "world/Generator.h"

#include <gtkmm/checkbutton.h>
#include "app/Global.h"
#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>

#include "app/simulation/Simulation.h"

class GeneratePanel : public Gtk::Dialog
{
public:
    GeneratePanel(Gtk::Window& parent, Global& settings, Simulation& simulation);

private:
    Global& settings;
    Simulation& simulation;
    Gtk::Box content{Gtk::Orientation::VERTICAL};

    Gtk::Grid grid;

    // Seed
    Gtk::Label seed_label{"Seed"};
    Gtk::Entry seed_entry;

    // Cell selectors
    Gtk::Label cells_label{"Include Cells"};
    Gtk::CheckButton white_cb{"White"};
    Gtk::CheckButton red_cb{"Red"};
    Gtk::CheckButton blue_cb{"Blue"};
    Gtk::CheckButton green_cb{"Green"};

    // Radius
    Gtk::Label radius_label{"Radius"};
    Gtk::SpinButton radius_spin;

    Gtk::Button generate_button{"Generate"};

    void on_generate_clicked();
};

#endif // GENERATE_PANEL_H