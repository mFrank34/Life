/*
 * File: View.h
 * Author: Michael Franks
 * Created: 2026-02-17
 * Last Edited: 2026-02-14
 * Description:
 */

#ifndef VIEW_H
#define VIEW_H

#include <gtkmm/drawingarea.h>
#include <gtkmm/gestureclick.h>
#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/eventcontrollermotion.h>

class World;

class View : public Gtk::DrawingArea
{
public:
    View(World& world);

private:
    World* world;

    int cell_size = 16;

    // camera (world space)
    double camera_x = 0.0;
    double camera_y = 0.0;
    double zoom = 1.0;

    // drag
    double drag_start_x = 0.0;
    double drag_start_y = 0.0;
    double camera_start_x = 0.0;
    double camera_start_y = 0.0;

    // mouse tracking (GTK4-required)
    double mouse_x = 0.0;
    double mouse_y = 0.0;

    Glib::RefPtr<Gtk::GestureClick> click_controller;
    Glib::RefPtr<Gtk::EventControllerScroll> scroll_controller;
    Glib::RefPtr<Gtk::EventControllerMotion> motion_controller;

    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int w, int h) const;

    void on_click(int n_press, double mx, double my);
    void on_release(int n_press, double mx, double my);

    void on_drag_begin(double start_x, double start_y);
    void on_drag_update(double offset_x, double offset_y);

    bool on_scroll(double dx, double dy);
    void zoom_at(double mx, double my, double dy);

    void create_Grid(const Cairo::RefPtr<Cairo::Context>& cr,
                     int width, int height,
                     int size) const;
};

#endif