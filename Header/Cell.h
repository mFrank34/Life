#ifndef CELL_H
#define CELL_H

class Cell
{
private:
    char type; // type of node
    bool alive; // it's alive
public:
    Cell();

    Cell(char type);

    char get_type() const;

    bool is_alive();

    void set_type(char new_type);

    void is_alive(bool is_alive);

    void update_cell();

    ~Cell();
};

#endif