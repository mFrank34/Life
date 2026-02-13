#include "Interface.h"
#include "View.h"
#include "world/World.h"
#include <iostream>

Interface::Interface(WorldBase& world, Settings& settings)
    : Gtk::Box(Gtk::Orientation::VERTICAL),
      world(world),
      settings(settings),
      view(world),
      last_tick(std::chrono::steady_clock::now())
{
    set_hexpand(true);
    set_vexpand(true);

    // --- OVERLAY ---
    overlay.set_hexpand(true);
    overlay.set_vexpand(true);
    append(overlay);

    // --- WORLD VIEW ---
    overlay.set_child(view);

    // --- LEFT PANEL (colour selection) ---
    left_panel.set_halign(Gtk::Align::START);
    left_panel.set_valign(Gtk::Align::START);
    left_panel.set_margin(8);
    left_panel.set_spacing(6);
    overlay.add_overlay(left_panel);

    btn_blue.add_css_class("color-btn");
    btn_red.add_css_class("color-btn");
    btn_green.add_css_class("color-btn");
    btn_white.add_css_class("color-btn");

    left_panel.append(btn_blue);
    left_panel.append(btn_red);
    left_panel.append(btn_green);
    left_panel.append(btn_white);

    btn_blue.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_color), CellColor::Blue));
    btn_red.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_color), CellColor::Red));
    btn_green.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_color), CellColor::Green));
    btn_white.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_color), CellColor::White));

    // --- RIGHT PANEL (speed controls) ---
    right_panel.set_halign(Gtk::Align::END);
    right_panel.set_valign(Gtk::Align::START);
    right_panel.set_margin(8);
    right_panel.set_spacing(6);
    overlay.add_overlay(right_panel);

    btn_speed_1.set_label("x1");
    btn_speed_2.set_label("x2");
    btn_speed_4.set_label("x4");
    btn_speed_8.set_label("x8");

    btn_speed_1.add_css_class("speed-btn");
    btn_speed_2.add_css_class("speed-btn");
    btn_speed_4.add_css_class("speed-btn");
    btn_speed_8.add_css_class("speed-btn");

    right_panel.append(btn_speed_1);
    right_panel.append(btn_speed_2);
    right_panel.append(btn_speed_4);
    right_panel.append(btn_speed_8);

    btn_speed_1.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_speed), SimSpeed::X1));
    btn_speed_2.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_speed), SimSpeed::X2));
    btn_speed_4.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_speed), SimSpeed::X4));
    btn_speed_8.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &Interface::on_speed), SimSpeed::X8));

    // --- BOTTOM CONTROLS ---
    bottom_controls.set_margin(8);
    bottom_controls.set_spacing(12);
    bottom_controls.set_halign(Gtk::Align::CENTER);
    bottom_controls.set_valign(Gtk::Align::END);
    overlay.add_overlay(bottom_controls);

    btn_start.set_label("Start");
    btn_pause.set_label("Pause");
    btn_restart.set_label("Restart");
    btn_generate.set_label("Generate");
    btn_rule_editor.set_label("Rule Editor");
    btn_import.set_label("Import");
    btn_export.set_label("Export");
    btn_settings.set_label("Settings");

    bottom_controls.append(btn_start);
    bottom_controls.append(btn_pause);
    bottom_controls.append(btn_restart);
    bottom_controls.append(btn_generate);
    bottom_controls.append(btn_rule_editor);
    bottom_controls.append(btn_import);
    bottom_controls.append(btn_export);
    bottom_controls.append(btn_settings);

    btn_start.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_start));
    btn_pause.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_pause));
    btn_restart.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_restart));
    btn_generate.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_generate));
    btn_rule_editor.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_rule_editor));
    btn_import.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_import));
    btn_export.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_export));
    btn_settings.signal_clicked().connect(sigc::mem_fun(*this, &Interface::on_settings));

    // Initialize UI state
    update_speed_ui();
    update_color_ui();
}

// ========== SIMULATION ==========

bool Interface::on_tick()
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> delta = now - last_tick;
    last_tick = now;

    double dt = delta.count();

    // Update world
    world.update(dt);

    // Trigger view redraw
    view.queue_draw();

    return true; // Keep timer running
}

void Interface::start_simulation()
{
    if (!is_running)
    {
        is_running = true;
        last_tick = std::chrono::steady_clock::now();

        sim_timer = Glib::signal_timeout().connect(
            sigc::mem_fun(*this, &Interface::on_tick),
            get_interval_ms()
        );
    }
}

void Interface::pause_simulation()
{
    if (is_running)
    {
        is_running = false;
        sim_timer.disconnect();
    }
}

void Interface::set_speed(SimSpeed speed)
{
    current_speed = speed;

    // If running, restart timer with new interval
    if (is_running)
    {
        sim_timer.disconnect();
        sim_timer = Glib::signal_timeout().connect(
            sigc::mem_fun(*this, &Interface::on_tick),
            get_interval_ms()
        );
    }

    update_speed_ui();
}

int Interface::get_interval_ms() const
{
    const int base_interval = 16; // ~60 FPS

    switch (current_speed)
    {
    case SimSpeed::X1: return base_interval;
    case SimSpeed::X2: return base_interval / 2;
    case SimSpeed::X4: return base_interval / 4;
    case SimSpeed::X8: return base_interval / 8;
    default: return base_interval;
    }
}

// ========== EVENT HANDLERS ==========

void Interface::on_start()
{
    start_simulation();
    std::cout << "Simulation started" << std::endl;
}

void Interface::on_pause()
{
    pause_simulation();
    std::cout << "Simulation paused" << std::endl;
}

void Interface::on_restart()
{
    pause_simulation();
    world.reset();
    view.queue_draw();
    std::cout << "Simulation restarted" << std::endl;
}

void Interface::on_generate()
{
    std::cout << "Generate clicked" << std::endl;
    // TODO: Open generation dialog or generate random world
}

void Interface::on_rule_editor()
{
    std::cout << "Rule editor clicked" << std::endl;
    // TODO: Open rule editor dialog
}

void Interface::on_import()
{
    std::cout << "Import clicked" << std::endl;
    // TODO: Open file dialog to import world/rules
}

void Interface::on_export()
{
    std::cout << "Export clicked" << std::endl;
    // TODO: Open file dialog to export world/rules
}

void Interface::on_settings()
{
    show_settings_dialog();
}

void Interface::on_speed(SimSpeed speed)
{
    set_speed(speed);
    std::cout << "Speed changed to: ";
    switch (speed)
    {
    case SimSpeed::X1: std::cout << "x1";
        break;
    case SimSpeed::X2: std::cout << "x2";
        break;
    case SimSpeed::X4: std::cout << "x4";
        break;
    case SimSpeed::X8: std::cout << "x8";
        break;
    }
    std::cout << std::endl;
}

void Interface::on_color(CellColor color)
{
    current_color = color;
    update_color_ui();

    std::cout << "Color changed to: ";
    switch (color)
    {
    case CellColor::Blue: std::cout << "Blue";
        break;
    case CellColor::Red: std::cout << "Red";
        break;
    case CellColor::Green: std::cout << "Green";
        break;
    case CellColor::White: std::cout << "White";
        break;
    }
    std::cout << std::endl;
}

// ========== UI UPDATES ==========

void Interface::update_speed_ui()
{
    // Remove "active" class from all speed buttons
    btn_speed_1.remove_css_class("active");
    btn_speed_2.remove_css_class("active");
    btn_speed_4.remove_css_class("active");
    btn_speed_8.remove_css_class("active");

    // Add "active" class to current speed button
    switch (current_speed)
    {
    case SimSpeed::X1: btn_speed_1.add_css_class("active");
        break;
    case SimSpeed::X2: btn_speed_2.add_css_class("active");
        break;
    case SimSpeed::X4: btn_speed_4.add_css_class("active");
        break;
    case SimSpeed::X8: btn_speed_8.add_css_class("active");
        break;
    }
}

void Interface::update_color_ui()
{
    // Remove "active" class from all color buttons
    btn_blue.remove_css_class("active");
    btn_red.remove_css_class("active");
    btn_green.remove_css_class("active");
    btn_white.remove_css_class("active");

    // Add "active" class to current color button
    switch (current_color)
    {
    case CellColor::Blue: btn_blue.add_css_class("active");
        break;
    case CellColor::Red: btn_red.add_css_class("active");
        break;
    case CellColor::Green: btn_green.add_css_class("active");
        break;
    case CellColor::White: btn_white.add_css_class("active");
        break;
    }
}

// ========== SETTINGS DIALOG ==========

void Interface::show_settings_dialog()
{
    auto dialog = new Gtk::Dialog("Settings", *dynamic_cast<Gtk::Window*>(get_root()), true);
    dialog->set_default_size(400, 300);

    auto content = dialog->get_content_area();

    // Storage type selection
    auto storage_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
    storage_box->set_margin(12);
    storage_box->set_spacing(12);

    auto storage_label = Gtk::make_managed<Gtk::Label>("Storage Type:");
    storage_box->append(*storage_label);

    auto storage_dropdown = Gtk::make_managed<Gtk::DropDown>();
    auto string_list = Gtk::StringList::create({"Sparse", "Cache", "Unordered"});
    storage_dropdown->set_model(string_list);

    // Set current selection
    switch (settings.storage_type)
    {
    case StorageType::Sparse: storage_dropdown->set_selected(0);
        break;
    case StorageType::Cache: storage_dropdown->set_selected(1);
        break;
    case StorageType::Unordered: storage_dropdown->set_selected(2);
        break;
    }

    storage_box->append(*storage_dropdown);
    content->append(*storage_box);

    // Add other settings here...

    // Buttons
    dialog->add_button("Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("Apply", Gtk::ResponseType::APPLY);

    dialog->signal_response().connect([this, dialog, storage_dropdown](int response)
    {
        if (response == Gtk::ResponseType::APPLY)
        {
            // Get selected storage type
            StorageType new_type;
            switch (storage_dropdown->get_selected())
            {
            case 0: new_type = StorageType::Sparse;
                break;
            case 1: new_type = StorageType::Cache;
                break;
            case 2: new_type = StorageType::Unordered;
                break;
            default: new_type = StorageType::Sparse;
            }

            // Emit signal to change storage type (Window will handle recreation)
            if (new_type != settings.storage_type)
            {
                signal_storage_changed.emit(new_type);
            }
        }

        delete dialog;
    });

    dialog->show();
}
