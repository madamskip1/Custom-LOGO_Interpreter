#pragma once
#include "Node.h"
#include "Expression.h"

class ClassAssignment :
    public Node
{
public:
    ClassAssignment();

    const void addExpression(std::unique_ptr<Expression> expression);

    std::size_t getExpressionsSize() const;
    Expression* getExpression(int index = 0) const;

private:
    std::vector<std::unique_ptr<Expression>> expressions;
};

