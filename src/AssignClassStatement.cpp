#include "AssignClassStatement.h"

AssignClassStatement::AssignClassStatement() : Node(NodeType::AssignClassStatement) {};

const void AssignClassStatement::addExpression(std::shared_ptr<Expression> expression)
{
    expressions.push_back(expression);
}

std::size_t AssignClassStatement::getExpressionsSize() const
{
    return expressions.size();
}

std::shared_ptr<Expression> AssignClassStatement::getExpression(int index) const
{
    return expressions[index];
}

std::vector<std::shared_ptr<Expression>> AssignClassStatement::getExpressions() const
{
    return expressions;
}
