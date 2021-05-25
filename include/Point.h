#ifndef POINT_H
#define POINT_H
#include "Variable.h"

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
};

#endif // POINT_H
