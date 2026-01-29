#include "Interface.h"
#include "View.h"
#include "World.h"
#include "Manager.h"

Interface::Interface(World& world, Manager& manager)
    : Gtk::Box(Gtk::Orientation::VERTICAL),
      world(world),
      manager(manager)
{
    set_hexpand(true);
    set_vexpand(true);

    // --- World view (expands) ---
    auto view = Gtk::make_managed<View>(world);
    view->set_hexpand(true);
    view->set_vexpand(true);

    append(*view);

    // --- Control bar (fixed height)
    auto controls = Gtk::make_managed<Gtk::Grid>();
    controls->set_hexpand(true);
    controls->set_vexpand(false);
    controls->set_column_spacing(12);
    controls->set_margin(8);

    append(*controls);


    // --- Cell type buttons ---
    auto cell_panel = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    cell_panel->set_vexpand(false);

    cell_panel->append(*Gtk::make_managed<Gtk::Button>("Blue"));
    cell_panel->append(*Gtk::make_managed<Gtk::Button>("Red"));
    cell_panel->append(*Gtk::make_managed<Gtk::Button>("Green"));
    cell_panel->append(*Gtk::make_managed<Gtk::Button>("White"));

    controls->attach(*cell_panel, 0, 0, 1, 2);


    // --- Simulation buttons ---
    auto sim_panel = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    sim_panel->set_vexpand(false);

    sim_panel->append(*Gtk::make_managed<Gtk::Button>("Start"));
    sim_panel->append(*Gtk::make_managed<Gtk::Button>("Pause"));
    sim_panel->append(*Gtk::make_managed<Gtk::Button>("Restart"));
    sim_panel->append(*Gtk::make_managed<Gtk::Button>("Generate"));

    controls->attach(*sim_panel, 1, 1);

    // --- Speed slider ----
    auto speed = Gtk::make_managed<Gtk::Scale>(Gtk::Orientation::VERTICAL);
    speed->set_range(1, 60);
    speed->set_value(10);
    speed->set_vexpand(true);

    controls->attach(*speed, 2, 0, 1, 2);
}

Gtk::Widget* Interface::create_start_page()
{
    auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    box->set_valign(Gtk::Align::CENTER);
    box->set_halign(Gtk::Align::CENTER);
    box->set_spacing(16);

    auto title = Gtk::make_managed<Gtk::Label>("Conway's Game of Life");
    title->set_margin_bottom(20);

    auto start = Gtk::make_managed<Gtk::Button>("Start");

    start->signal_clicked().connect([this]()
    {
        stack.set_visible_child("main");
    });

    box->append(*title);
    box->append(*start);

    return box;
}

// Gtk::Widget* Interface::create_main_ui()
// {
//     auto root = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
//
//     // ---- World Area ----
//     auto overlay = Gtk::make_managed<Gtk::Overlay>();
//     auto view = Gtk::make_managed<View>(world);
//     view->set_hexpand(true);
//     view->set_vexpand(true);
//     overlay->set_child(*view);
//     root->append(*overlay);
//
//     // ---- Controls ----
//     auto controls = Gtk::make_managed<Gtk::Grid>();
//     controls->set_row_spacing(8);
//     controls->set_column_spacing(16);
//     controls->set_margin(12);
//
//     // Cell type column
//     auto cell_panel = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
//     cell_panel->append(*Gtk::make_managed<Gtk::ToggleButton>("Blue"));
//     cell_panel->append(*Gtk::make_managed<Gtk::ToggleButton>("Red"));
//     cell_panel->append(*Gtk::make_managed<Gtk::ToggleButton>("Green"));
//     cell_panel->append(*Gtk::make_managed<Gtk::ToggleButton>("White"));
//
//     // Simulation buttons row
//     auto sim_panel = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
//     sim_panel->set_spacing(6);
//     sim_panel->append(*Gtk::make_managed<Gtk::Button>("Start"));
//     sim_panel->append(*Gtk::make_managed<Gtk::Button>("Pause"));
//     sim_panel->append(*Gtk::make_managed<Gtk::Button>("Restart"));
//     sim_panel->append(*Gtk::make_managed<Gtk::Button>("Generate"));
//
//     // Speed control
//     auto speed = Gtk::make_managed<Gtk::Scale>(Gtk::Orientation::VERTICAL);
//     speed->set_range(0.25, 5.0);
//     speed->set_value(1.0);
//     speed->set_vexpand(true);
//
//     // Place grid items
//     controls->attach(*cell_panel, 0, 0);
//     controls->attach(*sim_panel, 1, 0);
//     controls->attach(*speed, 2, 0);
//
//     root->append(*controls);
//     return root;
// }
