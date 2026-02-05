#include "View.h"

#include <gtkmm/gesturedrag.h>
#include <algorithm>
#include <cmath>

#include "World.h"
#include "Chunk.h"

View::View(World& world)
    : world(world)
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

    // motion (track mouse position — REQUIRED for GTK4 zoom)
    motion_controller = Gtk::EventControllerMotion::create();
    motion_controller->signal_motion().connect(
        [this](double x, double y)
        {
            mouse_x = x;
            mouse_y = y;
        }
    );
    add_controller(motion_controller);

    // scroll (zoom)
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

void View::on_draw(
    const Cairo::RefPtr<Cairo::Context>& cr,
    int width,
    int height
) const
{
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
                    // wall cell
                    cr->set_source_rgba(1.0, 1.0, 1.0, 1.0);
                }
                else
                {
                    // default cell
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

    // draw grid last
    create_Grid(cr, width, height, 1);
}

void View::on_click(int, double mx, double my)
{
    double world_x = camera_x + mx / zoom;
    double world_y = camera_y + my / zoom;

    int cx = world_x / cell_size;
    int cy = world_y / cell_size;

    auto& cell = world.get_cell(cx, cy);

    if (cell.get_type() == 'w')
        cell.set_type('0'); // unselect
    else
        cell.set_type('w'); // place wall

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
    double world_x = camera_x + mx / zoom;
    double world_y = camera_y + my / zoom;

    double zoom_factor = std::pow(1.1, -dy);
    double new_zoom = std::clamp(zoom * zoom_factor, 0.2, 5.0);

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
    // grid spacing in screen pixels
    double screen_step = (size * cell_size) * zoom;

    // too small to be meaningful
    if (screen_step < 6.0)
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

    cr->set_source_rgba(0.0, 0.5, 0.0, 0.5);
    cr->set_line_width(1.0 / zoom);

    for (int x = start_x; x <= end_x; x += step)
    {
        cr->move_to(x + 0.5, start_y + 0.5);
        cr->line_to(x + 0.5, end_y + 0.5);
    }

    for (int y = start_y; y <= end_y; y += step)
    {
        cr->move_to(start_x + 0.5, y + 0.5);
        cr->line_to(end_x + 0.5, y + 0.5);
    }

    cr->stroke();
}
