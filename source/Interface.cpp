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

    // --- OVERLAY (world view + floating panels) ---
    auto overlay = Gtk::make_managed<Gtk::Overlay>();
    overlay->set_hexpand(true);
    overlay->set_vexpand(true);
    append(*overlay);

    // --- WORLD VIEW ---
    auto view = Gtk::make_managed<View>(world);
    overlay->set_child(*view);

    // --- LEFT PANEL (floats top-left) ---
    auto left_panel = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    left_panel->set_halign(Gtk::Align::START);
    left_panel->set_valign(Gtk::Align::START);
    left_panel->set_margin(8);

    overlay->add_overlay(*left_panel);

    // Add your cell type buttons
    left_panel->append(*Gtk::make_managed<Gtk::Button>("Blue"));
    left_panel->append(*Gtk::make_managed<Gtk::Button>("Red"));
    left_panel->append(*Gtk::make_managed<Gtk::Button>("Green"));
    left_panel->append(*Gtk::make_managed<Gtk::Button>("White"));

    // --- RIGHT PANEL (floats top-right) ---
    auto right_panel = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
    right_panel->set_halign(Gtk::Align::END);
    right_panel->set_valign(Gtk::Align::START);
    right_panel->set_margin(8);

    overlay->add_overlay(*right_panel);

    // Speed buttons
    auto speed1 = Gtk::make_managed<Gtk::Button>("x1");
    auto speed2 = Gtk::make_managed<Gtk::Button>("x2");
    auto speed3 = Gtk::make_managed<Gtk::Button>("x4");
    auto speed4 = Gtk::make_managed<Gtk::Button>("x8");

    speed1->add_css_class("speed-btn");
    speed2->add_css_class("speed-btn");
    speed3->add_css_class("speed-btn");
    speed4->add_css_class("speed-btn");

    right_panel->append(*speed1);
    right_panel->append(*speed2);
    right_panel->append(*speed3);
    right_panel->append(*speed4);


    // --- BOTTOM CONTROLS (floating bottom center) ---
    auto bottom_controls = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    bottom_controls->set_margin(8);
    bottom_controls->set_spacing(12);

    // Align bottom center
    bottom_controls->set_halign(Gtk::Align::CENTER);
    bottom_controls->set_valign(Gtk::Align::END);

    overlay->add_overlay(*bottom_controls);

    bottom_controls->append(*Gtk::make_managed<Gtk::Button>("Start"));
    bottom_controls->append(*Gtk::make_managed<Gtk::Button>("Pause"));
    bottom_controls->append(*Gtk::make_managed<Gtk::Button>("Restart"));
    bottom_controls->append(*Gtk::make_managed<Gtk::Button>("Generate"));
    bottom_controls->append(*Gtk::make_managed<Gtk::Button>("Rule Editor"));
    bottom_controls->append(*Gtk::make_managed<Gtk::Button>("Import"));
    bottom_controls->append(*Gtk::make_managed<Gtk::Button>("Export"));
    bottom_controls->append(*Gtk::make_managed<Gtk::Button>("Settings"));

    append(*bottom_controls);
}
