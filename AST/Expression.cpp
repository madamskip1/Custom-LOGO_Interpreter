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


const void AST::Expression::addChildExpression(std::unique_ptr<AST::Expression> child)
{
    childrenExpressions.push_back(std::move(child));
}

const void AST::Expression::addOperator(const TokenType& op)
{
    operators.push_back(op);
}

const void AST::Expression::setNegativeOp(const bool& negative)
{
    negativeOperator = negative;
}


const bool AST::Expression::getNegativeOperator() const
{
    return negativeOperator;
}

const TokenType AST::Expression::getOperator(const int& index) const
{
    return operators[index];
}
