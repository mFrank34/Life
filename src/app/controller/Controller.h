#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <functional>
#include <gtkmm/drawingarea.h>
#include <gtkmm/gestureclick.h>
#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/eventcontrollermotion.h>

class Model;
class World;
class Cell;

class Controller
{
public:
    Controller(Model& model, Gtk::DrawingArea& drawing_area);

    // Getters for rendering
    double get_camera_x() const { return camera_x; }
    double get_camera_y() const { return camera_y; }
    double get_zoom() const { return zoom; }
    int get_cell_size() const { return cell_size; }

    // Model access for View
    Model& get_model() { return model; }
    const Model& get_model() const { return model; }

    // World access - currently returns cache, can be extended later for switching
    World& get_world();
    const World& get_world() const;

    // Cell type management
    void set_current_cell_type(char type) { current_cell_type = type; }
    char get_current_cell_type() const { return current_cell_type; }

    // Callback to trigger redraws
    void set_redraw_callback(std::function<void()> callback);

private:
    Model& model;
    Gtk::DrawingArea& drawing_area;

    // Camera state
    int cell_size = 16;
    double camera_x = 0.0;
    double camera_y = 0.0;
    double zoom = 1.0;

    // Current cell type to place
    char current_cell_type = 'w';

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