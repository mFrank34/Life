#include "Interface.h"
#include "World.h"

Interface::Interface(const World& world)
    : world(world)
{
    set_draw_func(sigc::mem_fun(*this, &Interface::on_draw));
}

void Interface::on_draw(
    const Cairo::RefPtr<Cairo::Context>& cr,
    int width,
    int height
)
{
    cr->set_source_rgb(1, 1, 1);
    cr->paint();

    // draw world data here
}
