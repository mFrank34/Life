// App.cpp
#include "app/App.h"
#include "app/controller/Controller.h"
#include "app/model/Model.h"
#include "app/view/View.h"

void run()
{
    View& View;

    Model& Model;

    Controller& Controller(View, Model);
};