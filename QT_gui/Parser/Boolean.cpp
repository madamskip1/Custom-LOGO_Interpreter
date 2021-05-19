#include "Boolean.h"

Boolean::Boolean(bool val) : value(val)
{
	nodeType = NodeType::Boolean;
}

const bool Boolean::getValue() const
{
	return value;
}

bool Boolean::evaluate() const
{
	return value;
}
