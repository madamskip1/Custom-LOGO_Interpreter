#include "Point.h"
#include <vector>
#include <string>
#include "Context.h"

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
    if (identifiers.size() == 0)
    {
        context->evaluateValues.push_back(x);
        context->evaluateValues.push_back(y);
        return;
    }

    if (identifiers[0] == "x")
    {
        context->evaluateValues.push_back(x);;
        return;
    }

    if (identifiers[0] == "y")
    {
        context->evaluateValues.push_back(y);
        return;
    }

    throw "identifier not recognized";
}

void Point::setSomeVal(std::vector<std::string> identifiers, Context *context)
{
    if (identifiers.size() != 1)
        throw "wrong level of identifier";

    if (identifiers[0] == "x")
    {
        x = std::get<int>(context->setVariant);
        context->setVariant = std::monostate{};
        return;
    }

    if (identifiers[0] == "y")
    {
        y = std::get<int>(context->setVariant);
        context->setVariant = std::monostate{};
        return;
    }

    throw "identifier not recognized";
}
