/*
 * File: RulePanel.cpp
 * Author: Michael Franks 
 * Description:
 */

#include "RulePanel.h"

RulePanel::RulePanel(Gtk::Window& parent, Settings& settings)
    : Gtk::Dialog("Settings", parent)
      , settings(settings)
{
    set_transient_for(parent);
    set_modal(false);
    set_title("Rule Editor");
    set_default_size(800, 600);

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