#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <functional>
#include <gtkmm/drawingarea.h>
#include <gtkmm/gestureclick.h>
#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/eventcontrollermotion.h>

class World;
class Cell;

class Controller
{
public:
    Controller(World& world, Gtk::DrawingArea& drawing_area);

    // Getters for rendering
    double get_camera_x() const { return camera_x; }
    double get_camera_y() const { return camera_y; }
    double get_zoom() const { return zoom; }
    int get_cell_size() const { return cell_size; }

    // World access for Interface
    World& get_world() { return world; }
    const World& get_world() const { return world; }

    // Callback to trigger redraws
    void set_redraw_callback(std::function<void()> callback);

private:
    World& world;
    Gtk::DrawingArea& drawing_area;

    // Camera state
    int cell_size = 16;
    double camera_x = 0.0;
    double camera_y = 0.0;
    double zoom = 1.0;

    // Drag state
    double drag_start_x = 0.0;
    double drag_start_y = 0.0;
    double camera_start_x = 0.0;
    double camera_start_y = 0.0;

    // Mouse tracking
    double mouse_x = 0.0;
    double mouse_y = 0.0;

    // Event controllers
    Glib::RefPtr<Gtk::GestureClick> click_controller;
    Glib::RefPtr<Gtk::EventControllerScroll> scroll_controller;
    Glib::RefPtr<Gtk::EventControllerMotion> motion_controller;

    // Redraw callback
    std::function<void()> redraw_callback;

    // Setup event handlers
    void setup_event_handlers();

    // Event handlers
    void on_click(int n_press, double mx, double my);
    void on_release(int n_press, double mx, double my);
    void on_drag_begin(double start_x, double start_y);
    void on_drag_update(double offset_x, double offset_y);
    bool on_scroll(double dx, double dy);

    // Helper methods
    void zoom_at(double mx, double my, double dy);
    void trigger_redraw();
};

#endif
