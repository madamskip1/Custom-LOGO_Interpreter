#include "Point.h"
#include <vector>
#include <string>

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

void Point::getSomeVal(std::vector<std::string> identifiers, Context *context)
{
    if (identifiers.size() != 1)
        throw "wrong level of identifier";

    if (identifiers[0] == "x")
    {
        context->returnVariant = x;
        return;
    }

    if (identifiers[0] == "y")
    {
        context->returnVariant = y;
        return;
    }

    throw "identifier not recognized";
}
