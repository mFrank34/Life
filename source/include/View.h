#ifndef VIEW_H
#define VIEW_H

#include <gtkmm/drawingarea.h>

class World;

class View : public Gtk::DrawingArea
{
public:
    View(World& world);

private:
    World& world;

    int cell_size = 13;
    int cam_x = 0;
    int cam_y = 0;

    void on_draw(
        const Cairo::RefPtr<Cairo::Context>& cr,
        int w, int h
    );
};

#endif //VIEW_H
