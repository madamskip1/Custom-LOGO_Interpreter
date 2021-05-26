#include "Brush.h"

const int Brush::DEFAULT_SIZE = 2;
const std::string Brush::DEFAULT_COLOR = "#3498DB";


Brush::Brush() : size(DEFAULT_SIZE), color(DEFAULT_COLOR)
{
}

int Brush::getSize() const
{
    return size;
}

std::string Brush::getColor() const
{
    return color;
}

std::string Brush::getColorR() const
{
    return color.substr(1, 2);
}

std::string Brush::getColorG() const
{
    return color.substr(3, 2);
}

std::string Brush::getColorB() const
{
    return color.substr(5, 2);
}
