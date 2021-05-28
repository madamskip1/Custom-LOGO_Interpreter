#include "Boolean.h"
#include "Context.h"

AST::Boolean::Boolean(bool val) : value(val)
{
	nodeType = NodeType::Boolean;
}

const bool AST::Boolean::getValue() const
{
    return (negativeOperator ? !value : value);
}

bool AST::Boolean::evaluate() const
{
    return getValue();
}

void AST::Boolean::evaluate(Context* context)
{
    context->evaluateValue = getValue();
}
