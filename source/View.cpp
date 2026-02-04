#include "View.h"

#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/gesture.h>
#include <gtkmm/gestureclick.h>
#include <gtkmm/gesturedrag.h>

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

    // drag
    auto drag = Gtk::GestureDrag::create();
    drag->set_button(3);
    drag->signal_drag_begin().connect(sigc::mem_fun(*this, &View::on_drag_begin));
    drag->signal_drag_update().connect(sigc::mem_fun(*this, &View::on_drag_update));
    add_controller(drag);
}


void View::on_draw(
    const Cairo::RefPtr<Cairo::Context>& cr,
    int width,
    int height
) const
{
    // Apply camera transform ONCE
    cr->translate(-camera_x, -camera_y);
    cr->scale(zoom, zoom);

    // Draw grid under everything
    int size = 3;
    create_Grid(cr, width, height, size);

    // Draw world data
    auto& data = world.get_world();
    for (const auto& [key, chunk] : data)
    {
        bool even = ((chunk.get_CX() + chunk.get_CY()) % 2 == 0);

        const double r = even ? 0.8 : 0.4;
        const double g = 0.4;
        const double b = even ? 0.4 : 0.8;

        for (int cy = 0; cy < chunk.get_size(); cy++)
        {
            for (int cx = 0; cx < chunk.get_size(); cx++)
            {
                int world_x = chunk.get_CX() * chunk.get_size() + cx;
                int world_y = chunk.get_CY() * chunk.get_size() + cy;

                int px = world_x * cell_size;
                int py = world_y * cell_size;

                cr->set_source_rgb(r, g, b);
                cr->rectangle(px, py, cell_size, cell_size);
                cr->fill();
            }
        }
    }
}


void View::on_click(int n_press, double mx, double my)
{
    int button = click_controller->get_current_button();

    if (button == 1) // left mouse button
    {
        double world_x = (mx + camera_x) / zoom;
        double world_y = (my + camera_y) / zoom;

        int world_cell_x = world_x / cell_size;
        int world_cell_y = world_y / cell_size;

        auto& cell = world.get_cell(world_cell_x, world_cell_y);
        cell.set_type('w');
        queue_draw();
    }
}

void View::on_release(int n_press, double mx, double my)
{
    right_mouse_down = false;
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

void View::create_Grid(
    const Cairo::RefPtr<Cairo::Context>& cr,
    int width,
    int height,
    int size
) const
{
    // visible world area
    double left = camera_x;
    double top = camera_y;
    double right = camera_x + width / zoom;
    double bottom = camera_y + height / zoom;

    int chunk_pixels = size * cell_size;

    int start_x = std::floor(left / chunk_pixels) * chunk_pixels;
    int end_x = std::ceil(right / chunk_pixels) * chunk_pixels;

    int start_y = std::floor(top / chunk_pixels) * chunk_pixels;
    int end_y = std::ceil(bottom / chunk_pixels) * chunk_pixels;

    cr->set_source_rgba(0, 128, 0, 0.5);
    cr->set_line_width(2.0 / zoom);

    for (int x = start_x; x <= end_x; x += chunk_pixels)
    {
        cr->move_to(x, start_y);
        cr->line_to(x, end_y);
    }

    for (int y = start_y; y <= end_y; y += chunk_pixels)
    {
        cr->move_to(start_x, y);
        cr->line_to(end_x, y);
    }

    cr->stroke();
}
