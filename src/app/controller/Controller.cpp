// Controller.cpp

#include "Controller.h"
#include "app/model/Model.h"
#include "world/World.h"
#include <gtkmm/gesturedrag.h>
#include <algorithm>
#include <cmath>

Controller::Controller(Model& model, Gtk::DrawingArea& drawing_area)
    : model(model),
      drawing_area(drawing_area)
{
    setup_event_handlers();
}

World& Controller::get_world()
{
    // For now, just use cache. Can extend later for switching between storage types
    return model.getCache();
}

const World& Controller::get_world() const
{
    // For now, just use cache. Can extend later for switching between storage types
    return model.getCache();
}

void Controller::setup_event_handlers()
{
    // Click
    click_controller = Gtk::GestureClick::create();
    click_controller->signal_pressed().connect(
        sigc::mem_fun(*this, &Controller::on_click)
    );
    click_controller->signal_released().connect(
        sigc::mem_fun(*this, &Controller::on_release)
    );
    drawing_area.add_controller(click_controller);

    // Drag (RMB)
    auto drag = Gtk::GestureDrag::create();
    drag->set_button(3);
    drag->signal_drag_begin().connect(
        sigc::mem_fun(*this, &Controller::on_drag_begin)
    );
    drag->signal_drag_update().connect(
        sigc::mem_fun(*this, &Controller::on_drag_update)
    );
    drawing_area.add_controller(drag);

    // Motion (track mouse position)
    motion_controller = Gtk::EventControllerMotion::create();
    motion_controller->signal_motion().connect(
        [this](double x, double y)
        {
            mouse_x = x;
            mouse_y = y;
        }
    );
    drawing_area.add_controller(motion_controller);

    // Scroll (zoom)
    scroll_controller = Gtk::EventControllerScroll::create();
    scroll_controller->set_flags(
        Gtk::EventControllerScroll::Flags::VERTICAL
    );
    scroll_controller->signal_scroll().connect(
        sigc::mem_fun(*this, &Controller::on_scroll),
        false
    );
    drawing_area.add_controller(scroll_controller);
}

void Controller::set_redraw_callback(std::function<void()> callback)
{
    redraw_callback = callback;
}

void Controller::on_click(int, double mx, double my)
{
    double world_x = camera_x + mx / zoom;
    double world_y = camera_y + my / zoom;

    int cx = world_x / cell_size;
    int cy = world_y / cell_size;

    auto& cell = get_world().get_cell(cx, cy);

    // If clicking on the same type, clear it; otherwise set to current type
    if (cell.get_type() == current_cell_type)
        cell.set_type('0'); // clear cell
    else
        cell.set_type(current_cell_type); // place current color

    trigger_redraw();
}

void Controller::on_release(int, double mx, double my)
{
    // Handle release if needed
}

void Controller::on_drag_begin(double start_x, double start_y)
{
    drag_start_x = start_x;
    drag_start_y = start_y;
    camera_start_x = camera_x;
    camera_start_y = camera_y;
}

void Controller::on_drag_update(double offset_x, double offset_y)
{
    camera_x = camera_start_x - offset_x / zoom;
    camera_y = camera_start_y - offset_y / zoom;
    trigger_redraw();
}

bool Controller::on_scroll(double, double dy)
{
    zoom_at(mouse_x, mouse_y, dy);
    return true;
}

void Controller::zoom_at(double mx, double my, double dy)
{
    // TODO Fix the zoom limiting section and fix bug with placement of cells when moving zoom location
    double zoom_factor = std::pow(1.1, -dy);
    double new_zoom = std::clamp(zoom * zoom_factor, 0.2, 5.0);

    if (new_zoom == zoom)
        return;

    double world_x = camera_x + mx / zoom;
    double world_y = camera_y + my / zoom;

    camera_x = world_x - mx / new_zoom;
    camera_y = world_y - my / new_zoom;

    zoom = new_zoom;
    trigger_redraw();
}

void Controller::trigger_redraw()
{
    if (redraw_callback)
        redraw_callback();
}