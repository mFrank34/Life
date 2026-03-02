/*
 * File: SettingPanel.cpp
 * Author: Michael Franks
 * Description: Settings panel for world configuration and benchmarking
 */

#include "SettingPanel.h"

#include "app/Global.h"
#include "app/simulation/Simulation.h"
#include "profiler/Profiler.h"
#include "world/storage/Cache.h"
#include "world/storage/Sparse.h"
#include "world/storage/Unordered.h"

SettingPanel::SettingPanel(Gtk::Window& parent, Global& settings, Simulation& simulation)
    : settings(settings), simulation(simulation)
{
    set_transient_for(parent);
    set_modal(true);
    set_resizable(false);
    set_destroy_with_parent(true);

    content.set_margin(12);
    content.set_spacing(16);
    content.set_hexpand(true);

    grid.set_row_spacing(8);
    grid.set_column_spacing(12);

    // ---- World type dropdown ----
    world_label.set_xalign(0.0f);

    auto world_options = Gtk::StringList::create({"Sparse", "Cache", "Unordered"});
    world_dropdown.set_model(world_options);
    world_dropdown.set_selected(0);
    world_dropdown.set_hexpand(true);

    grid.attach(world_label, 0, 0);
    grid.attach(world_dropdown, 1, 0);

    // ---- Chunk size ----
    chunk_label.set_xalign(0.0f);

    chunk_spin.set_range(1, 128);
    chunk_spin.set_increments(1, 8);
    chunk_spin.set_digits(0);
    chunk_spin.set_numeric(true);
    chunk_spin.set_value(32);
    chunk_spin.set_hexpand(true);

    grid.attach(chunk_label, 0, 1);
    grid.attach(chunk_spin, 1, 1);

    // ---- Buttons ----
    apply_button.set_hexpand(true);
    apply_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SettingPanel::on_apply_clicked)
    );

    benchmark_button.set_hexpand(true);
    benchmark_button.add_css_class("suggested-action");
    benchmark_button.signal_clicked().connect(
        sigc::mem_fun(*this, &SettingPanel::on_benchmark_clicked)
    );

    button_box.set_spacing(8);
    button_box.append(apply_button);
    button_box.append(benchmark_button);

    content.append(grid);
    content.append(button_box);
    get_content_area()->append(content);

    signal_close_request().connect([this]() -> bool
    {
        hide();
        return true;
    }, false);
}

void SettingPanel::on_apply_clicked()
{
    auto selected = std::dynamic_pointer_cast<Gtk::StringObject>(
        world_dropdown.get_selected_item()
    );

    std::string type = selected ? selected->get_string() : "Sparse";
    int chunk_size = (int)chunk_spin.get_value();

    // Swap world via Global
    if (type == "Sparse")
        settings.set_world(std::make_unique<Sparse>(chunk_size));
    else if (type == "Cache")
        settings.set_world(std::make_unique<Cache>(chunk_size, 256));
    else if (type == "Unordered")
        settings.set_world(std::make_unique<Unordered>());

    simulation.attach_world(*settings.get_world());
    hide();
}

void SettingPanel::on_benchmark_clicked()
{
    auto selected = std::dynamic_pointer_cast<Gtk::StringObject>(
        world_dropdown.get_selected_item()
    );

    std::string type = selected ? selected->get_string() : "Sparse";
    int chunk_size = (int)chunk_spin.get_value();

    // Pause and wait for any in-progress update to finish before swapping
    simulation.pause();
    while (simulation.is_updating())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // Swap world
    if (type == "Sparse")
        settings.set_world(std::make_unique<Sparse>(chunk_size));
    else if (type == "Cache")
        settings.set_world(std::make_unique<Cache>(chunk_size, 256));
    else if (type == "Unordered")
        settings.set_world(std::make_unique<Unordered>());

    simulation.attach_world(*settings.get_world());

    GeneratorRequest request;
    request.seed = "benchmark";
    request.radius = 64;
    request.use_white = true;
    request.use_red = true;
    request.use_blue = true;
    request.use_green = true;

    std::string filename = "profile_" + type + ".json";
    simulation.start_benchmark(request, filename);

    hide();
}