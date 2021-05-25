#include "Number.h"
#include "../include/Context.h"

AST::Number::Number(int value) : value(value)
{
    nodeType = AST::NodeType::Number;
}

int AST::Number::evaluate(Context* context) const
{
    return (negativeOperator ? (value * -1) : value);
}

const int AST::Number::getValue() const
{
    return value;
}
