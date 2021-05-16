#include "Color.h"

Color::Color(std::string color) : colorHex(color)
{ 
	nodeType = NodeType::Color;
}

const void Color::setColor(std::string color)
{
	colorHex = color;
}

std::string Color::getColor() const
{
	return colorHex;
}
