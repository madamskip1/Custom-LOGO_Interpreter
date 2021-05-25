#ifndef POINT_H
#define POINT_H
#include <vector>
#include <string>
#include "Variable.h"

class Context;

class Point
        : public Variable
{
public:
    Point();
    Point(const Point& point);
    Point(int pos);
    Point(int x, int y);
    int x;
    int y;

    void getSomeVal(std::vector<std::string> identifiers, Context* context);
};

#endif // POINT_H
