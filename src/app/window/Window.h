#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm/window.h>
#include <memory>
#include "Interface.h"
#include "world/World.h"
#include "app/Settings.h"

// Forward declarations for your storage types
class SparseStorage;
class CacheStorage;
class UnorderedStorage;

// Base class for World (if you have one)
// If you don't have a WorldBase, you'll need to use std::variant instead
class WorldBase
{
public:
    virtual ~WorldBase() = default;
    virtual void update(double dt) = 0;
    virtual void reset() = 0;
    // Add other common methods your World classes share
};

class Window : public Gtk::Window
{
public:
    Window();

    Settings& get_settings() { return settings; }

private:
    void initialize_world();
    void on_storage_type_changed(StorageType new_type);

    Settings settings;
    std::unique_ptr<WorldBase> world;
    std::unique_ptr<Interface> interface;
};

#endif // WINDOW_H
