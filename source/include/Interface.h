#ifndef INTERFACE_H
#define INTERFACE_H

#pragma once
#include <gtkmm.h>

class World;

class Interface : public Gtk::DrawingArea
{
public:
    explicit Interface(const World& world);

private:
    const World& world;

    int cell_size = 8;
    int cam_x = 0;
    int cam_y = 0;

    bool dragging = false;
    double last_x = 0;
    double last_y = 0;

    void on_draw(
        const Cairo::RefPtr<Cairo::Context>& cr,
        int width,
        int height
    );
};


#endif // INTERFACE_H
