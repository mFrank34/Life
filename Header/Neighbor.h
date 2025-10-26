#ifndef NEIGHBOR_H
#define NEIGHBOR_H

#include <string>
#include <format>
#include <vector>

class Neighbor
{
private:
    int global_x, global_y;

public:
    Neighbor(int global_x, int global_y);

    bool operator==(const Neighbor &self) const;

    int hash_code();

    std::string to_string();

    std::vector<Neighbor> neighbors_cords();

    ~Neighbor();
};

#endif