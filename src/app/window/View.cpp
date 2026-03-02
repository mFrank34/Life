/*
 * File: View.cpp
 * Author: Michael Franks
 * Description: world view
 */

#include "View.h"

#include <gtkmm/gesturedrag.h>
#include <algorithm>
#include <cmath>
#include <giomm/settings.h>

#include "app/Global.h"
#include "app/simulation/Simulation.h"
#include "profiler/Profiler.h"
#include "world/World.h"
#include "world/structure/Chunk.h"

View::View(World& world, Simulation& simulation, Global& settings)
    : world(&world), simulation(simulation), settings(settings)
{
    set_hexpand(true);
    set_vexpand(true);
    set_focusable(true);
    set_can_target(true);

    set_draw_func(sigc::mem_fun(*this, &View::on_draw));

    // click
    click_controller = Gtk::GestureClick::create();
    click_controller->signal_pressed().connect(
        sigc::mem_fun(*this, &View::on_click)
    );
    click_controller->signal_released().connect(
        sigc::mem_fun(*this, &View::on_release)
    );
    add_controller(click_controller);

    // drag (RMB)
    auto drag = Gtk::GestureDrag::create();
    drag->set_button(3);
    drag->signal_drag_begin().connect(
        sigc::mem_fun(*this, &View::on_drag_begin)
    );
    drag->signal_drag_update().connect(
        sigc::mem_fun(*this, &View::on_drag_update)
    );
    add_controller(drag);

    // motion
    motion_controller = Gtk::EventControllerMotion::create();
    motion_controller->signal_motion().connect(
        [this](double x, double y)
        {
            mouse_x = x;
            mouse_y = y;
        }
    );
    add_controller(motion_controller);

    // scroll
    scroll_controller = Gtk::EventControllerScroll::create();
    scroll_controller->set_flags(
        Gtk::EventControllerScroll::Flags::VERTICAL
    );
    scroll_controller->signal_scroll().connect(
        sigc::mem_fun(*this, &View::on_scroll),
        false
    );
    add_controller(scroll_controller);
}

void View::attach_world(World& new_world)
{
    std::lock_guard lock(world_ptr_mtx);
    world = &new_world;
}

void View::on_draw(
    const Cairo::RefPtr<Cairo::Context>& cr,
    int width,
    int height
) const
{
    // Safely read world pointer
    World* w = nullptr;
    {
        std::lock_guard lock(world_ptr_mtx);
        w = world;
    }
    if (!w) return;

    Perf::Profiler::get().begin_render();
    std::shared_lock lock(w->world_mtx);

    cr->scale(zoom, zoom);
    cr->translate(-camera_x, -camera_y);

    float view_left = camera_x;
    float view_top = camera_y;
    float view_right = camera_x + (width / zoom);
    float view_bottom = camera_y + (height / zoom);

    auto& data = w->get_world();

    // Batch rectangles per color to minimize Cairo state changes and fill calls
    std::unordered_map<uint8_t, std::vector<std::tuple<float, float, float, float>>> batches;

    for (const auto& [key, chunk] : data)
    {
        int chunk_size_px = w->get_size() * cell_size;
        int chunk_x_px = chunk.get_CX() * chunk_size_px;
        int chunk_y_px = chunk.get_CY() * chunk_size_px;

        if (chunk_x_px + chunk_size_px < view_left ||
            chunk_x_px > view_right ||
            chunk_y_px + chunk_size_px < view_top ||
            chunk_y_px > view_bottom)
            continue;

        for (int cy = 0; cy < chunk.get_size(); cy++)
        {
            for (int cx = 0; cx < chunk.get_size(); cx++)
            {
                const Cell& cell = chunk.get_cell(cx, cy);

                int wx = chunk.get_CX() * chunk.get_size() + cx;
                int wy = chunk.get_CY() * chunk.get_size() + cy;

                float cell_x = wx * cell_size;
                float cell_y = wy * cell_size;

                uint8_t type = static_cast<uint8_t>(cell.get_type());
                batches[type].emplace_back(cell_x, cell_y, cell_size, cell_size);
            }
        }
    }

    // Draw each color batch in one pass
    auto draw_batch = [&](uint8_t type, double r, double g, double b, double a)
    {
        auto it = batches.find(type);
        if (it == batches.end()) return;

        cr->set_source_rgba(r, g, b, a);
        for (auto& [x, y, ww, h] : it->second)
            cr->rectangle(x, y, ww, h);
        cr->fill();
    };

    draw_batch(static_cast<uint8_t>(CellType::White), 1.0, 1.0, 1.0, 1.0);
    draw_batch(static_cast<uint8_t>(CellType::Blue), 0.2, 0.4, 0.9, 1.0);
    draw_batch(static_cast<uint8_t>(CellType::Green), 0.2, 0.8, 0.2, 1.0);
    draw_batch(static_cast<uint8_t>(CellType::Red), 0.9, 0.2, 0.2, 1.0);

    // Chunk debug overlay
    if (settings.debug)
    {
        cr->set_line_width(2.0 / zoom);

        for (const auto& [key, chunk] : data)
        {
            int chunk_size_px = chunk.get_size() * cell_size;
            int chunk_x_px = chunk.get_CX() * chunk_size_px;
            int chunk_y_px = chunk.get_CY() * chunk_size_px;

            if (chunk_x_px + chunk_size_px < view_left ||
                chunk_x_px > view_right ||
                chunk_y_px + chunk_size_px < view_top ||
                chunk_y_px > view_bottom)
                continue;

            cr->set_source_rgba(1.0, 1.0, 0.0, 0.6);
            cr->rectangle(chunk_x_px, chunk_y_px, chunk_size_px, chunk_size_px);
            cr->stroke();

            if (zoom > 0.3)
            {
                cr->set_source_rgba(1.0, 1.0, 0.0, 1.0);
                cr->set_font_size(8.0 / zoom);
                cr->move_to(chunk_x_px + 2.0 / zoom, chunk_y_px + 10.0 / zoom);

                std::string label =
                    "(" + std::to_string(chunk.get_CX()) +
                    "," + std::to_string(chunk.get_CY()) + ")";

                cr->show_text(label);
            }
        }
    }

    create_Grid(cr, width, height, 1);
    Perf::Profiler::get().end_render(); // end of full draw
}

void View::on_click(int, double mx, double my)
{
    World* w = nullptr;
    {
        std::lock_guard lock(world_ptr_mtx);
        w = world;
    }
    if (!w) return;

    simulation.pause();
    double world_x = camera_x + mx / zoom;
    double world_y = camera_y + my / zoom;

    int cx = static_cast<int>(std::floor(world_x / cell_size));
    int cy = static_cast<int>(std::floor(world_y / cell_size));

    auto& cell = w->get_cell(cx, cy);

    if (cell.get_type() == settings.colour)
        cell.set_type(CellType::Empty);
    else
        cell.set_type(settings.colour);

    queue_draw();
}

void View::on_release(int, double, double)
{
}

void View::on_drag_begin(double start_x, double start_y)
{
    drag_start_x = start_x;
    drag_start_y = start_y;
    camera_start_x = camera_x;
    camera_start_y = camera_y;
}

void View::on_drag_update(double offset_x, double offset_y)
{
    camera_x = camera_start_x - offset_x / zoom;
    camera_y = camera_start_y - offset_y / zoom;
    queue_draw();
}

bool View::on_scroll(double, double dy)
{
    zoom_at(mouse_x, mouse_y, dy);
    return true;
}

void View::zoom_at(double mx, double my, double dy)
{
    double zoom_factor = std::pow(1.1, -dy);
    double new_zoom = std::clamp(zoom * zoom_factor, 0.2, 5.0);

    if (new_zoom == zoom)
        return;

    double world_x = camera_x + mx / zoom;
    double world_y = camera_y + my / zoom;

    camera_x = world_x - mx / new_zoom;
    camera_y = world_y - my / new_zoom;

    zoom = new_zoom;
    queue_draw();
}

void View::create_Grid(
    const Cairo::RefPtr<Cairo::Context>& cr,
    int width,
    int height,
    int size
) const
{
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

    cr->set_source_rgba(0.3, 0.3, 0.3, 0.4);
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
