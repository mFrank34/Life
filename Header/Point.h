#ifndef POINT_H
#define POINT_H

#include <string>
#include <format>
#include <vector>

class Point
{
private:
    int x, y;

public:
    Point(int x, int y);

    bool operator==(const Point &self) const;

    int hash_code();

    std::string to_string();

    std::vector<Point> neighbors();

    ~Point();
};

#endif