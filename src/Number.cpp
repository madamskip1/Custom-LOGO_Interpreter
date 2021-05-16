#include "Number.h"

Number::Number(int value) : value(value)
{
    nodeType = NodeType::Number;
}

int Number::evaluate() const
{
    return (negativeOperator ? (value * -1) : value);
}

const int Number::getValue() const
{
    return value;
}
