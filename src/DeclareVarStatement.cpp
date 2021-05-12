#include "DeclareVarStatement.h"

DeclareVarStatement::DeclareVarStatement() : Node(NodeType::DeclareVarStatement) {};

const void DeclareVarStatement::setType(const TokenType& type)
{
    this->type = type;
}

const void DeclareVarStatement::setIdentifier(std::string id)
{
    identifier = id;
}

const void DeclareVarStatement::setAssignStatement(std::shared_ptr<AssignStatement> assign)
{
    assignStatement = assign;
}

const void DeclareVarStatement::setAssignClassStatement(std::shared_ptr<AssignClassStatement> assignClass)
{
    assignClassStatement = assignClass;
}

const void DeclareVarStatement::setColorVal(std::string color)
{
    colorVal = color;
}

TokenType DeclareVarStatement::getType() const
{
    return type;
}

std::string DeclareVarStatement::getIdentifier() const
{
    return identifier;
}

std::shared_ptr<AssignStatement> DeclareVarStatement::getAssignStatemnt() const
{
    return assignStatement;
}

std::shared_ptr<AssignClassStatement> DeclareVarStatement::getAssignClassStatement() const
{
    return assignClassStatement;
}

std::string DeclareVarStatement::getColorVal() const
{
    return colorVal;
}
