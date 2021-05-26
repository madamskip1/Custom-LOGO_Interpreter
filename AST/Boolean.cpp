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

int AST::Boolean::evaluate(Context* context) const
{
    context->evaluateValues.push_back(value);
    return 0;
}
