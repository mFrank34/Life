/*
 * File: GeneratePanel.cpp
 * Author: Michael Franks
 * Description: system to generate random patten across a multiple chunks
 */

#include "GeneratePanel.h"

GeneratePanel::GeneratePanel(Gtk::Window& parent, Settings& settings, Simulation& simulation)
    : Gtk::Dialog("Generator", parent)
      , settings(settings)
      , simulation(simulation)
{
    set_transient_for(parent);
    set_modal(true);
    set_resizable(false);
    set_destroy_with_parent(true);

    // Content box
    content.set_margin(12);
    content.set_spacing(16);
    content.set_hexpand(true);

    // Grid layout
    grid.set_row_spacing(8);
    grid.set_column_spacing(12);

    // ---- Seed row ----
    seed_label.set_xalign(0.0f);
    seed_entry.set_placeholder_text("Random or numeric seed");
    seed_entry.set_hexpand(true);

    grid.attach(seed_label, 0, 0);
    grid.attach(seed_entry, 1, 0);

    // ---- Cell selector ----
    cells_label.set_xalign(0.0f);
    grid.attach(cells_label, 0, 1);

    auto cell_box = Gtk::make_managed<Gtk::Box>(
        Gtk::Orientation::HORIZONTAL, 12
    );
    cell_box->set_halign(Gtk::Align::START);

    cell_box->append(white_cb);
    cell_box->append(red_cb);
    cell_box->append(blue_cb);
    cell_box->append(green_cb);

    // Default selections
    white_cb.set_active(true);
    red_cb.set_active(true);
    blue_cb.set_active(true);
    green_cb.set_active(true);

    grid.attach(*cell_box, 1, 1);

    // ---- Radius ----
    radius_label.set_xalign(0.0f);

    radius_spin.set_range(1, 10000);
    radius_spin.set_increments(1, 5);
    radius_spin.set_digits(0);
    radius_spin.set_numeric(true);
    radius_spin.set_value(5);
    radius_spin.set_width_chars(4);

    grid.attach(radius_label, 0, 2);
    grid.attach(radius_spin, 1, 2);

    // Generate button
    generate_button.set_hexpand(true);
    generate_button.add_css_class("generate-button");

    generate_button.signal_clicked().connect(
        sigc::mem_fun(*this, &GeneratePanel::on_generate_clicked)
    );

    // Assemble layout
    content.append(grid);
    content.append(generate_button);
    get_content_area()->append(content);

    // Close behavior
    signal_close_request().connect(
        [this]() -> bool
        {
            hide();
            return true;
        },
        false
    );
}

void GeneratePanel::on_generate_clicked()
{
    GeneratorRequest request;
    request.seed = seed_entry.get_text();
    request.radius = (int)radius_spin.get_value();

    request.use_white = white_cb.get_active();
    request.use_red = red_cb.get_active();
    request.use_blue = blue_cb.get_active();
    request.use_green = green_cb.get_active();

    simulation.generate(request);
    hide();
}