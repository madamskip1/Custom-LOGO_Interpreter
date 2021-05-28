#include "Expression.h"
#include "../include/Context.h"

AST::Expression::Expression()
{
    nodeType = NodeType::Expression;
}

bool AST::Expression::isOnlyId() const
{
    if (childrenExpressions.size() != 1)
        return false;

    return childrenExpressions[0]->isOnlyId();
}

std::vector<std::string> AST::Expression::getIdentifiers() const
{
    if (childrenExpressions.size() != 1)
        throw "expression isn't just identifiers";

    return childrenExpressions[0]->getIdentifiers();
}


void AST::Expression::addChildExpression(std::unique_ptr<AST::Expression> child)
{
    childrenExpressions.push_back(std::move(child));
}

void AST::Expression::addOperator(const TokenType& op)
{
    operators.push_back(op);
}

void AST::Expression::setNegativeOp(const bool& negative)
{
    negativeOperator = negative;
}

size_t AST::Expression::getChildrenExpressionSize() const
{
    return childrenExpressions.size();
}

AST::Expression *AST::Expression::getChildExpression(const int &index) const
{
    return childrenExpressions[index].get();
}

bool AST::Expression::getNegativeOperator() const
{
    return negativeOperator;
}

TokenType AST::Expression::getOperator(const int& index) const
{
    return operators[index];
}
