/*
 * File: RuleEditorView
 * Author: Michael Franks 
 * Description: control
 */

#ifndef RULE_PANEL_H
#define RULE_PANEL_H

#include <gtkmm/dialog.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>

#include "app/Global.h"

class RulePanel : public Gtk::Dialog
{
public:
    RulePanel(Gtk::Window& parent, Global& settings);
    ~RulePanel() = default;

private:
    static void build_grid(
        Gtk::Grid& grid,
        std::array<Gtk::SpinButton, 4>& cells
    );

    void make_row(
        Gtk::Label& row_label,
        std::array<Gtk::SpinButton, 4>& cells,
        int row_index
    );

    static void load_row(
        const Rule& rule,
        std::array<Gtk::SpinButton, 4>& cells
    );

    void load_rules();

    static void save_row(
        Rule& rule,
        std::array<Gtk::SpinButton, 4>& cells
    );

    void apply_rules();

    Global& settings;
    Gtk::Box content{Gtk::Orientation::VERTICAL};
    Gtk::Box rules{Gtk::Orientation::VERTICAL, 12};
    Gtk::Grid grid;

    Gtk::Label h_surv_min{"Survival Min"};
    Gtk::Label h_surv_max{"Survival Max"};
    Gtk::Label h_birth_min{"Birth Min"};
    Gtk::Label h_birth_max{"Birth Max"};

    Gtk::Box white_row{Gtk::Orientation::HORIZONTAL, 12};
    Gtk::Box red_row{Gtk::Orientation::HORIZONTAL, 12};
    Gtk::Box blue_row{Gtk::Orientation::HORIZONTAL, 12};
    Gtk::Box green_row{Gtk::Orientation::HORIZONTAL, 12};

    Gtk::Label white_label{"White Cell"};
    Gtk::Label red_label{"Red Cell"};
    Gtk::Label blue_label{"Blue Cell"};
    Gtk::Label green_label{"Green Cells"};

    std::array<Gtk::SpinButton, 4> white_cells;
    std::array<Gtk::SpinButton, 4> red_cells;
    std::array<Gtk::SpinButton, 4> blue_cells;
    std::array<Gtk::SpinButton, 4> green_cells;

    Gtk::Grid white_grid;
    Gtk::Grid red_grid;
    Gtk::Grid blue_grid;
    Gtk::Grid green_grid;

    std::array<Gtk::SpinButton, 4> cells;

    Gtk::Button submit_button{"Apply Rules"};
};

#endif // RULE_PANEL_H