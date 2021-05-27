#include "Boolean.h"
#include "Context.h"

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

void AST::Boolean::evaluate(Context* context) const
{
    context->evaluateValue = value;
}
