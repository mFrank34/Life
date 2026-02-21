/*
 * File: SettingView
 * Author: Michael Franks 
 * Description: the setting page for application
 */

#ifndef SETTING_PANEL_H
#define SETTING_PANEL_H

#include "app/Settings.h"
#include <gtkmm/dialog.h>

class SettingPanel : public Gtk::Dialog
{
public:
    SettingPanel(Gtk::Window& parent, Settings& settings);
    ~SettingPanel() = default;

private:
    Settings& settings;
    Gtk::Box content{Gtk::Orientation::VERTICAL};
};

#endif // SETTING_PANEL_H
