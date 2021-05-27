#include "Color.h"
#include "Context.h"

AST::Color::Color(std::string color) : colorHex(color)
{ 
    nodeType = NodeType::Color;
}

void AST::Color::evaluate(Context* context) const
{
    context->evaluateValue = getColor();
}

const void AST::Color::setColor(std::string color)
{
	colorHex = color;
}

std::string AST::Color::getColor() const
{
	return colorHex;
}
