#ifndef NEIGHBOR_H
#define NEIGHBOR_H

#include <string>
#include <format>
#include <vector>

class Neighbor
{
private:
    int x, y;

public:
    Neighbor(int x, int y);

    bool operator==(const Neighbor &self) const;

    int hash_code();

    std::string to_string();

    std::vector<Neighbor> neighbors();

    ~Neighbor();
};

#endif