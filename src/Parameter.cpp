#include "Parameter.h"

Parameter::Parameter() : Node(NodeType::Parameter) {};

const void Parameter::setName(const std::string paramName)
{
    name = paramName;
}

const void Parameter::setType(const TokenType type)
{
    this->type = type;
}

std::string Parameter::getName() const
{
    return name;
}

TokenType Parameter::getType() const
{
    return type;
}
