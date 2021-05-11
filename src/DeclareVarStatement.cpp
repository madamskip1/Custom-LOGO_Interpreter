#include "DeclareVarStatement.h"

const void DeclareVarStatement::setType(TokenType type)
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
