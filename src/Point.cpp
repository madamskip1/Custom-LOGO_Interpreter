#include "Point.h"

Point::Point()
{
    x = y = 0;
}

Point::Point(const Point& point)
{
    x = point.x;
    y = point.y;
}

Point::Point(int pos)
{
    x = y = pos;
}

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}
