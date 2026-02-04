#ifndef VIEW_H
#define VIEW_H

#include <gtkmm/drawingarea.h>
#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/gestureclick.h>

class World;

class View : public Gtk::DrawingArea
{
public:
    View(World& world);

private:
    World& world;

    int cell_size = 6;

    double camera_x = 0.0;
    double camera_y = 0.0;

    double zoom = 1.0;

    double drag_start_x = 0;
    double drag_start_y = 0;
    double camera_start_x = 0;
    double camera_start_y = 0;

    bool right_mouse_down = false;
    Glib::RefPtr<Gtk::GestureClick> click_controller;

    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int w, int h) const;

    void on_click(int n_press, double mx, double my);
    void on_release(int n_press, double mx, double my);
    void on_drag_begin(double start_x, double start_y);
    void on_drag_update(double offset_x, double offset_y);

    void create_Grid(const Cairo::RefPtr<Cairo::Context>& cr,
                     int width, int height,
                     int size) const;
};

#endif // VIEW_H
