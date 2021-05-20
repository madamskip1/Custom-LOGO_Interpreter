#include "Parameter.h"

Parameter::Parameter(TokenType parameterType, std::string parameterName) : Node(NodeType::Parameter)
{
    type = parameterType;
    name = parameterName;
}

std::string Parameter::getName() const
{
    return name;
}

TokenType Parameter::getType() const
{
    return type;
}
