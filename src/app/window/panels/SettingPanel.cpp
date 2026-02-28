/*
 * File: SettingView
 * Author: Michael Franks 
 * Description:
 */

#include "SettingPanel.h"

SettingPanel::SettingPanel(Gtk::Window& parent, Global& settings)
    : settings(settings)
{
    set_transient_for(parent);
    set_modal(false);
    set_resizable(false);
    set_title("Settings");

    content.set_margin(12);
    content.set_spacing(12);
    content.set_hexpand(true);
    content.set_vexpand(true);

    get_content_area()->append(content);

    signal_close_request().connect([this]() -> bool
    {
        hide();
        return true;
    }, false);
}