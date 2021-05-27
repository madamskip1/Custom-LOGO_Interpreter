#include "Number.h"
#include "../include/Context.h"

AST::Number::Number(int value) : value(value)
{
    nodeType = AST::NodeType::Number;
}

void AST::Number::evaluate(Context* context)
{
    int val = (negativeOperator ? (value * -1) : value);
    context->evaluateValue = val;
}

const int AST::Number::getValue() const
{
    return value;
}
