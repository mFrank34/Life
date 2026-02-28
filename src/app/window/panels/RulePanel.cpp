/*
 * File: RulePanel.cpp
 * Author: Michael Franks 
 * Description:
 */

#include "RulePanel.h"

#include "rules/Rules.h"

RulePanel::RulePanel(Gtk::Window& parent, Global& settings)
    : Gtk::Dialog("Rule Editor", parent)
      , settings(settings)
{
    set_transient_for(parent);
    set_modal(true);
    set_destroy_with_parent(true);

    // Main content box
    content.set_margin(12);
    content.set_spacing(16);
    content.set_hexpand(true);
    content.set_vexpand(true);

    // Main grid (headers + rows)
    grid.set_row_spacing(8);
    grid.set_column_spacing(16);
    grid.set_halign(Gtk::Align::CENTER);
    grid.set_valign(Gtk::Align::START);
    grid.set_column_homogeneous(true);

    // --- Column headers ---
    Gtk::Label spacer{""};
    spacer.set_xalign(0.0f);
    grid.attach(spacer, 0, 0);
    grid.set_column_spacing(8);
    grid.set_halign(Gtk::Align::START);

    grid.attach(h_surv_min, 1, 0);
    grid.attach(h_surv_max, 2, 0);
    grid.attach(h_birth_min, 3, 0);
    grid.attach(h_birth_max, 4, 0);

    // Add rows
    make_row(white_label, white_cells, 1);
    make_row(red_label, red_cells, 2);
    make_row(blue_label, blue_cells, 3);
    make_row(green_label, green_cells, 4);

    content.append(grid);
    get_content_area()->append(content);

    load_rules();

    submit_button.set_halign(Gtk::Align::CENTER);
    submit_button.signal_clicked().connect([this]()
    {
        apply_rules();
        hide();
    });

    content.append(submit_button);

    signal_show().connect([this]()
    {
        load_rules();
    });

    signal_close_request().connect(
        [this]() -> bool
        {
            hide();
            return true;
        },
        false
    );
}

void RulePanel::build_grid(
    Gtk::Grid& grid,
    std::array<Gtk::SpinButton, 4>& cells
)
{
    for (int c = 0; c < 4; ++c)
    {
        auto& spin = cells[c];

        spin.set_range(0, 999);
        spin.set_increments(1, 1);
        spin.set_digits(0);
        spin.set_numeric(true);
        spin.set_width_chars(2); // compact

        grid.attach(spin, c, 0);
    }
}

void RulePanel::make_row(
    Gtk::Label& row_label,
    std::array<Gtk::SpinButton, 4>& cells,
    int row_index
)
{
    row_label.set_xalign(0.0f); // left-align row label
    grid.attach(row_label, 0, row_index); // first column

    for (int c = 0; c < 4; ++c)
    {
        auto& spin = cells[c];
        spin.set_range(0, 999);
        spin.set_increments(1, 1);
        spin.set_digits(0);
        spin.set_numeric(true);
        spin.set_width_chars(2);
        grid.attach(spin, c + 1, row_index); // columns 1-4
    }
}

void RulePanel::load_row(
    const Rule& rule,
    std::array<Gtk::SpinButton, 4>& cells
)
{
    cells[0].set_value(rule.survival_min);
    cells[1].set_value(rule.survival_max);
    cells[2].set_value(rule.birth_min);
    cells[3].set_value(rule.birth_max);
}

void RulePanel::load_rules()
{
    load_row(settings.rules.white, white_cells);
    load_row(settings.rules.red, red_cells);
    load_row(settings.rules.blue, blue_cells);
    load_row(settings.rules.green, green_cells);
}

void RulePanel::save_row(
    Rule& rule,
    std::array<Gtk::SpinButton, 4>& cells
)
{
    rule.survival_min = cells[0].get_value_as_int();
    rule.survival_max = cells[1].get_value_as_int();
    rule.birth_min = cells[2].get_value_as_int();
    rule.birth_max = cells[3].get_value_as_int();
}

void RulePanel::apply_rules()
{
    save_row(settings.rules.white, white_cells);
    save_row(settings.rules.red, red_cells);
    save_row(settings.rules.blue, blue_cells);
    save_row(settings.rules.green, green_cells);
}