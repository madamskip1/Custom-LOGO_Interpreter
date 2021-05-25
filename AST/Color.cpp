#include "Color.h"

AST::Color::Color(std::string color) : colorHex(color)
{ 
	nodeType = NodeType::Color;
}

const void AST::Color::setColor(std::string color)
{
	colorHex = color;
}

std::string AST::Color::getColor() const
{
	return colorHex;
}
