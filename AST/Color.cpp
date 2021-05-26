#include "Color.h"
#include "Context.h"

AST::Color::Color(std::string color) : colorHex(color)
{ 
    nodeType = NodeType::Color;
}

int AST::Color::evaluate(Context* context) const
{
    context->evaluateValues.push_back(getColor());
    return 0;
}

const void AST::Color::setColor(std::string color)
{
	colorHex = color;
}

std::string AST::Color::getColor() const
{
	return colorHex;
}
