/*
 * File: RuleEditorView
 * Author: Michael Franks 
 * Description:
 */

#ifndef RULE_PANEL_H
#define RULE_PANEL_H

#include <gtkmm/dialog.h>
#include "app/Settings.h"

class RulePanel : public Gtk::Dialog
{
public:
    RulePanel(Gtk::Window& parent, Settings& settings);
    ~RulePanel() = default;

private:
    Settings& settings;
    Gtk::Box content{Gtk::Orientation::VERTICAL};
};
#endif // RULE_PANEL_H