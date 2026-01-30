#include "../Neighbor.h"
#include <format>
#include <vector>

Neighbor::Neighbor(int input_x, int input_y)
{
    this->global_x = input_x;
    this->global_y = input_y;
}

bool Neighbor::operator==(const Neighbor &self) const // its looking at it self
{
    return global_x == self.global_x && global_y == self.global_y;
    // takes two value and see if they match and returns true and false
}

int Neighbor::hash_code()
{
    return (global_x * 3) + (global_y * 5);
}

std::string Neighbor::to_string()
{
    // we want to return this as string (x,global_y)
    std::string coords = std::format("({},{})", global_x, global_y);
    return coords;
}

std::vector<Neighbor> Neighbor::neighbors_cords()
{
    return {
        {global_x - 1, global_y - 1},
        {global_x - 1, global_y},
        {global_x - 1, global_y + 1},
        {global_x, global_y - 1},
        {global_x, global_y + 1},
        {global_x + 1, global_y - 1},
        {global_x + 1, global_y},
        {global_x + 1, global_y + 1}};
    /*
    this finds the titles around the current Neighbor
    and helps to find where it can move to? if that makes sense?
    */
}

Neighbor::~Neighbor() {};

/* Chunks lay out: starting from 0 to 7 for 8 entries
   * 1 2 3    * is chunk that being updated
   * 4 * 5    - need to find out chunks that are
   * 6 7 8    - alive within world
   */