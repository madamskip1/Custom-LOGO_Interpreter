#include "Number.h"
#include "../include/Context.h"

AST::Number::Number(int value) : value(value)
{
    nodeType = AST::NodeType::Number;
}

int AST::Number::evaluate(Context* context) const
{
    int val = (negativeOperator ? (value * -1) : value);
    context->evaluateValue = val;
    return val;
}

const int AST::Number::getValue() const
{
    return value;
}
