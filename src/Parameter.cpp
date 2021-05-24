#include "Parameter.h"

AST::Parameter::Parameter(TokenType parameterType, std::string parameterName) : AST::Node(AST::NodeType::Parameter)
{
    type = parameterType;
    name = parameterName;
}

std::string AST::Parameter::getName() const
{
    return name;
}

TokenType AST::Parameter::getType() const
{
    return type;
}
