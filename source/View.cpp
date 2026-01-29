#include "View.h"
#include "World.h"

View::View(World& world)
    : world(world)
{
    set_hexpand(true);
    set_vexpand(true);

    set_draw_func(sigc::mem_fun(*this, &View::on_draw));
}

void View::on_draw(
    const Cairo::RefPtr<Cairo::Context>& cr,
    int width,
    int height
)
{
    // background
    cr->set_source_rgb(0.05, 0.05, 0.05);
    cr->paint();

    // placeholder grid (proof it works)
    cr->set_source_rgb(0.2, 0.2, 0.2);

    for (int x = 0; x < width; x += cell_size)
    {
        cr->move_to(x, 0);
        cr->line_to(x, height);
    }

    for (int y = 0; y < height; y += cell_size)
    {
        cr->move_to(0, y);
        cr->line_to(width, y);
    }

    cr->stroke();
}
