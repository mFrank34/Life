#include "Window.h"
#include "world/storage/Sparse.h"
#include "world/storage/Cache.h"
#include "world/storage/Unordered.h"
#include <iostream>

Window::Window()
{
    set_title("Simulation");
    set_default_size(800, 600);

    initialize_world();
}

void Window::initialize_world()
{
    // Stop any running simulation
    if (interface)
    {
        interface->pause_simulation();
    }

    // Create the appropriate World type based on settings
    switch (settings.storage_type)
    {
    case StorageType::Sparse:
        world = std::make_unique<World<Sparse>>(32);
        std::cout << "Created Sparse world" << std::endl;
        break;

    case StorageType::Cache:
        world = std::make_unique<World<Cache>>(32, 16);
        std::cout << "Created Cache world" << std::endl;
        break;

    case StorageType::Unordered:
        world = std::make_unique<World<Unordered>>(1);
        std::cout << "Created Unordered world" << std::endl;
        break;
    }

    // Create new interface with the new world
    interface = std::make_unique<Interface>(*world, settings);

    // Connect storage type change signal
    interface->signal_storage_changed.connect(
        sigc::mem_fun(*this, &Window::on_storage_type_changed));

    set_child(*interface);
}

void Window::on_storage_type_changed(StorageType new_type)
{
    if (settings.storage_type != new_type)
    {
        std::cout << "Changing storage type from "
            << Settings::storage_type_to_string(settings.storage_type)
            << " to "
            << Settings::storage_type_to_string(new_type)
            << std::endl;

        settings.storage_type = new_type;
        initialize_world(); // Recreate everything with new storage type
    }
}
