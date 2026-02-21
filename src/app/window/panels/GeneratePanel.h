/*
 * File: Generate
 * Author: Michael Franks 
 * Description: view for generate section of application
 */

#ifndef GENERATE_PANEL_H
#define GENERATE_PANEL_H

#include "app/Settings.h"
#include <gtkmm/dialog.h>

class GeneratePanel : public Gtk::Dialog
{
public:
    GeneratePanel(Gtk::Window& parent, Settings& settings);
    ~GeneratePanel() = default;

private:
    Settings& settings;
    Gtk::Box content{Gtk::Orientation::VERTICAL};
};

#endif // GENERATE_PANEL_H