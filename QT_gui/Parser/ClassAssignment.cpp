#include "ClassAssignment.h"

ClassAssignment::ClassAssignment() : Node(NodeType::AssignClassStatement) {};

const void ClassAssignment::addExpression(std::unique_ptr<Expression> expression)
{
    expressions.push_back(std::move(expression));
}

std::size_t ClassAssignment::getExpressionsSize() const
{
    return expressions.size();
}

Expression* ClassAssignment::getExpression(int index) const
{
    return expressions[index].get();
}


