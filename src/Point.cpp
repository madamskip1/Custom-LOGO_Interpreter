#include "Point.h"
#include <vector>
#include <string>
#include "Context.h"

Point::Point()
{
    x = y = 0;
    type = TokenType::Point;
}

Point::Point(const Point& point)
{
    x = point.x;
    y = point.y;
    type = TokenType::Point;
}

Point::Point(int pos)
{
    x = y = pos;
    type = TokenType::Point;
}

Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
    type = TokenType::Point;
}

void Point::getSomeVal(std::vector<std::string> identifiers, Context *context)
{
    if (identifiers.size() == 0)
    {
        context->evaluateValue = this;
        return;
    }

    if (identifiers[0] == "x")
    {
        context->evaluateValue = x;
        return;
    }

    if (identifiers[0] == "y")
    {
        context->evaluateValue = y;
        return;
    }

    throw "identifier not recognized";
}

void Point::setSomeVal(std::vector<std::string> identifiers, Context *context)
{
    if (identifiers.size() == 0)
    {
        if (context->evaluateValue.index() != 4)
            throw "wrong Point class assignment";

        Variable* pointVar = std::get<Variable*>(context->evaluateValue);

        if (pointVar->type != TokenType::Point)
            throw "Only Point type can be assign to Point var";

        Point* point = static_cast<Point*>(pointVar);
        x = point->x;
        y = point->y;
        return;
    }
    if (identifiers.size() > 1)
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
