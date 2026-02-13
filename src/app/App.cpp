// App.cpp
#include "app/App.h"
#include "app/controller/Controller.h"
#include "app/model/Model.h"
#include "app/view/View.h"

void run()
{
    Model model;
    View view;

    Controller controller(model, view.getDrawingArea());

    view.setController(&controller);

    controller.set_redraw_callback([&view]()
    {
        view.queue_draw();
    });
};
