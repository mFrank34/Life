#pragma once
#ifndef CELL_H
#define CELL_H

class Cell
{
private:
    char type;
    bool alive{};
public:
    Cell();

    Cell(char type);

    char get_type() const;

    bool is_alive() const;

    void set_type(char new_type);

    void set_alive(bool is_alive);

    void update_cell();

    ~Cell();
};

#endif