#include "Boolean.h"

AST::Boolean::Boolean(bool val) : value(val)
{
	nodeType = NodeType::Boolean;
}

const bool AST::Boolean::getValue() const
{
	return value;
}

bool AST::Boolean::evaluate() const
{
	return value;
}
