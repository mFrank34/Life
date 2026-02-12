// View.h
#include "View.h"
#include "app/controller/Controller.h"
#include "world/World.h"
#include "world/structure/Chunk.h"
#include <cmath>

View::View()
    : Gtk::Box(Gtk::Orientation::VERTICAL)
{
    set_hexpand(true);
    set_vexpand(true);

    setup_ui();
}

void View::set_controller(Controller* ctrl)
{
    controller = ctrl;

    if (controller)
    {
        // Setup controller callback to trigger redraws
        controller->set_redraw_callback([this]()
        {
            drawing_area.queue_draw();
        });
    }
}

void View::set_manager(Manager* mgr)
{
    manager = mgr;
}

void View::setup_ui()
{
    // --- OVERLAY ---
    overlay.set_hexpand(true);
    overlay.set_vexpand(true);
    append(overlay);

    // --- DRAWING AREA SETUP ---
    drawing_area.set_hexpand(true);
    drawing_area.set_vexpand(true);
    drawing_area.set_focusable(true);
    drawing_area.set_can_target(true);
    drawing_area.set_draw_func(sigc::mem_fun(*this, &View::on_draw));
    overlay.set_child(drawing_area);

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
        sigc::bind(sigc::mem_fun(*this, &View::on_color), CellColor::Blue));
    btn_red.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &View::on_color), CellColor::Red));
    btn_green.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &View::on_color), CellColor::Green));
    btn_white.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &View::on_color), CellColor::White));

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
        sigc::bind(sigc::mem_fun(*this, &View::on_speed), SimSpeed::X1));
    btn_speed_2.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &View::on_speed), SimSpeed::X2));
    btn_speed_4.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &View::on_speed), SimSpeed::X4));
    btn_speed_8.signal_clicked().connect(
        sigc::bind(sigc::mem_fun(*this, &View::on_speed), SimSpeed::X8));

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

    btn_start.signal_clicked().connect(sigc::mem_fun(*this, &View::on_start));
    btn_pause.signal_clicked().connect(sigc::mem_fun(*this, &View::on_pause));
    btn_restart.signal_clicked().connect(sigc::mem_fun(*this, &View::on_restart));
    btn_generate.signal_clicked().connect(sigc::mem_fun(*this, &View::on_generate));
    btn_rule_editor.signal_clicked().connect(sigc::mem_fun(*this, &View::on_rule_editor));
    btn_import.signal_clicked().connect(sigc::mem_fun(*this, &View::on_import));
    btn_export.signal_clicked().connect(sigc::mem_fun(*this, &View::on_export));
    btn_settings.signal_clicked().connect(sigc::mem_fun(*this, &View::on_settings));
}

// --- DRAWING (with null checks) ---

void View::on_draw(
    const Cairo::RefPtr<Cairo::Context>& cr,
    int width,
    int height
)
{
    if (!controller)
        return; // Can't draw without controller

    double camera_x = controller->get_camera_x();
    double camera_y = controller->get_camera_y();
    double zoom = controller->get_zoom();
    int cell_size = controller->get_cell_size();

    // Get world data through controller
    auto& world = controller->get_world();

    cr->scale(zoom, zoom);
    cr->translate(-camera_x, -camera_y);

    auto& data = world.get_world();
    for (const auto& [key, chunk] : data)
    {
        bool even = ((chunk.get_CX() + chunk.get_CY()) % 2 == 0);

        for (int cy = 0; cy < chunk.get_size(); cy++)
        {
            for (int cx = 0; cx < chunk.get_size(); cx++)
            {
                int wx = chunk.get_CX() * chunk.get_size() + cx;
                int wy = chunk.get_CY() * chunk.get_size() + cy;

                char type = world.get_cell(wx, wy).get_type();

                if (type == 'w')
                {
                    cr->set_source_rgba(1.0, 1.0, 1.0, 1.0);
                }
                else
                {
                    cr->set_source_rgba(
                        even ? 0.8 : 0.4,
                        0.4,
                        even ? 0.4 : 0.8,
                        1.0
                    );
                }

                cr->rectangle(
                    wx * cell_size,
                    wy * cell_size,
                    cell_size,
                    cell_size
                );
                cr->fill();
            }
        }
    }
    create_Grid(cr, width, height, 1);
}

void View::create_Grid(
    const Cairo::RefPtr<Cairo::Context>& cr,
    int width,
    int height,
    int size
) const
{
    if (!controller)
        return;

    double camera_x = controller->get_camera_x();
    double camera_y = controller->get_camera_y();
    double zoom = controller->get_zoom();
    int cell_size = controller->get_cell_size();

    double screen_step = (size * cell_size) * zoom;

    if (screen_step < 5.0)
        return;

    double left = camera_x;
    double top = camera_y;
    double right = camera_x + width / zoom;
    double bottom = camera_y + height / zoom;

    int step = size * cell_size;

    int start_x = std::floor(left / step) * step;
    int end_x = std::ceil(right / step) * step;
    int start_y = std::floor(top / step) * step;
    int end_y = std::ceil(bottom / step) * step;

    cr->set_source_rgba(83, 83, 83, 0.5);
    cr->set_line_width(1.0 / zoom);

    for (int x = start_x; x <= end_x; x += step)
    {
        cr->move_to(x, start_y);
        cr->line_to(x, end_y);
    }

    for (int y = start_y; y <= end_y; y += step)
    {
        cr->move_to(start_x, y);
        cr->line_to(end_x, y);
    }

    cr->stroke();
}

// --- BUTTON CALLBACKS ---

void View::on_start()
{
    if (controller)
    {
        // controller->start_simulation();
    }
}

void View::on_pause()
{
    if (controller)
    {
        // controller->pause_simulation();
    }
}

void View::on_restart()
{
    if (controller)
    {
        // controller->restart_simulation();
    }
}

void View::on_generate()
{
    if (controller)
    {
        // controller->generate_world();
    }
}

void View::on_rule_editor()
{
    // Open rule editor dialog
}

void View::on_import()
{
    // Open file dialog
}

void View::on_export()
{
    // Open save dialog
}

void View::on_settings()
{
    // Open settings dialog
}

void View::on_speed(SimSpeed speed)
{
    if (controller)
    {
        // controller->set_simulation_speed(speed);
    }
}

void View::update_speed_ui()
{
    // Update which speed button appears active
}

void View::on_color(CellColor color)
{
    if (controller)
    {
        // controller->set_selected_color(color);
    }
}

void View::update_color_ui()
{
    // Update which color button appears active
}
