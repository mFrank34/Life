#ifndef CELL_H
#define CELL_H

class Cell
{
private:
    char type; // type of node
    bool node; // is it a node or not
public:
    Cell();

    Cell(char type);

    char get_type();

    bool get_node();

    void set_type(char new_type);

    void set_node(bool new_node);

    void update_cell();

    ~Cell();
};

#endif