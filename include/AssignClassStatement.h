#pragma once
#include <vector>
#include "Node.h"
#include "Expression.h"

class AssignClassStatement :
    public Node
{
public:
    AssignClassStatement() : Node(NodeType::AssignClassStatement) {};

    const void addExpression(std::shared_ptr<Expression> expression);

    std::size_t getExpressionsSize() const;
    std::shared_ptr<Expression> getExpression(int index = 0) const;
    std::vector<std::shared_ptr<Expression>> getExpressions() const;

private:
    std::vector<std::shared_ptr<Expression>> expressions;
};

