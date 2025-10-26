#include "Neighbor.h"
#include <format>
#include <vector>

Neighbor::Neighbor(int x, int y)
{
    this->x = x;
    this->y = y;
}

bool Neighbor::operator==(const Neighbor &self) const // its looking at it self
{
    return x == self.x && y == self.y;
    // takes two value and see if they match and returns true and false
}

int Neighbor::hash_code()
{
    return (x * 3) + (y * 5);
}

std::string Neighbor::to_string()
{
    // we want to return this as string (x,y)
    std::string coords = std::format("({},{})", x, y);
    return coords;
}

std::vector<Neighbor> Neighbor::neighbors()
{
    return {
        {x - 1, y - 1},
        {x - 1, y},
        {x - 1, y + 1},
        {x, y - 1},
        {x, y + 1},
        {x + 1, y - 1},
        {x + 1, y},
        {x + 1, y + 1}
    };
    /*
    this finds the titles around the current Neighbor
    and helps to find where it can move to? if that makes sense?
    */
}

Neighbor::~Neighbor() {};