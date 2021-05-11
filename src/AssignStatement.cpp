#include "AssignStatement.h"

const void AssignStatement::addIdentifier(std::string name)
{
    identifiers.push_back(name);
}

std::string AssignStatement::getIdentifier(int index) const
{
    return identifiers[index];
}

const void AssignStatement::setExpression(std::shared_ptr<Expression> expr)
{
    expression = expr;
}

std::shared_ptr<Expression> AssignStatement::getExpression() const
{
    return expression;
}
