/*
* File: SettingPanel.h
 * Author: Michael Franks
 * Description: Settings panel for world configuration and benchmarking
 */

#pragma once
#ifndef SETTINGPANEL_H
#define SETTINGPANEL_H

#include <gtkmm.h>
#include <functional>

#include "app/Global.h"
#include "app/simulation/Simulation.h"

class SettingPanel : public Gtk::Dialog
{
public:
    // Callbacks
    std::function<void(const std::string& type, int chunk_size)> on_world_changed;
    std::function<void()> on_benchmark_start;

    SettingPanel(Gtk::Window& parent, Global& settings, Simulation& simulation);

private:
    void on_apply_clicked();
    void on_benchmark_clicked();

    Global& settings;
    Simulation& simulation;

    Gtk::Box content{Gtk::Orientation::VERTICAL};
    Gtk::Grid grid;

    // World type
    Gtk::Label world_label{"World Type"};
    Gtk::DropDown world_dropdown;

    // Chunk size
    Gtk::Label chunk_label{"Chunk Size"};
    Gtk::SpinButton chunk_spin;

    // Buttons
    Gtk::Box button_box{Gtk::Orientation::HORIZONTAL};
    Gtk::Button apply_button{"Apply"};
    Gtk::Button benchmark_button{"Run Benchmark (2 min)"};
};

#endif
