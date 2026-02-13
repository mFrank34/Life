#pragma once
#ifndef VIEW_H
#define VIEW_H

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>

// Consistent enum names
enum class CellColor { BLUE, RED, GREEN, WHITE };
enum class SimSpeed { X1 = 1, X2 = 2, X4 = 4, X8 = 8 };

class Controller;

class View : public Gtk::Box
{
public:
    View();
    ~View() = default;

    // Setting the controller
    void setController(Controller* controller);

    // Access to drawing area for Controller setup
    Gtk::DrawingArea& getDrawingArea() { return drawing_area; }

private:
    Controller* controller = nullptr;

    // Current state
    CellColor current_color = CellColor::WHITE;
    SimSpeed current_speed = SimSpeed::X1;

    // UI components
    Gtk::Overlay overlay;
    Gtk::DrawingArea drawing_area;

    // Drawing handler
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int w, int h);
    void create_Grid(const Cairo::RefPtr<Cairo::Context>& cr,
                     int width, int height, int size);

    // Bottom controls
    Gtk::Box bottom_controls{Gtk::Orientation::HORIZONTAL};

    Gtk::Button btn_start;
    Gtk::Button btn_pause;
    Gtk::Button btn_restart;
    Gtk::Button btn_generate;
    Gtk::Button btn_rule_editor;
    Gtk::Button btn_import;
    Gtk::Button btn_export;
    Gtk::Button btn_settings;

    // Button callbacks
    void on_start();
    void on_pause();
    void on_restart();
    void on_generate();
    void on_rule_editor();
    void on_import();
    void on_export();
    void on_settings();

    // Right panel (speed control)
    Gtk::Box right_panel{Gtk::Orientation::VERTICAL};

    Gtk::Button btn_speed_1;
    Gtk::Button btn_speed_2;
    Gtk::Button btn_speed_4;
    Gtk::Button btn_speed_8;

    void on_speed(SimSpeed speed);
    void update_speed_ui();

    // Left panel (colour selection)
    Gtk::Box left_panel{Gtk::Orientation::VERTICAL};

    Gtk::Button btn_blue{"Blue"};
    Gtk::Button btn_red{"Red"};
    Gtk::Button btn_green{"Green"};
    Gtk::Button btn_white{"White"};

    void on_color(CellColor color);
    void update_color_ui();

    // Helper to initialize UI components
    void setup_ui();
};

#endif // VIEW_H