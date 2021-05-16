#include "Expression.h"

Expression::Expression() 
{
    nodeType = NodeType::Expression;
}

int Expression::evaluate() const
{
    int val = childrenExpressions[0]->evaluate();
    int val2 = 0;
    TokenType op;

    for (int i = 1; i < childrenExpressions.size(); i++)
    {
        val2 = childrenExpressions[i]->evaluate();
        op = operators[i - 1];

        if (op == TokenType::Plus)
            val += val2;
        else if (op == TokenType::Minus)
            val -= val2;
        else if (op == TokenType::Multiply)
            val *= val2;
        else if (op == TokenType::Divide)
            val /= val2;
    }

    if (negativeOperator)
        val *= -1;

    return val;
}


const void Expression::addChildExpression(std::unique_ptr<Expression> child)
{
    childrenExpressions.push_back(std::move(child));
}

const void Expression::addOperator(const TokenType& op)
{
    operators.push_back(op);
}

const void Expression::setNegativeOp(const bool& negative)
{
    negativeOperator = negative;
}

const std::size_t Expression::getChildrenExpressionSize() const
{
    return childrenExpressions.size();
}

Expression* Expression::getChildExpression(const int& index) const
{
    return childrenExpressions[index].get();
}

const bool Expression::getNegativeOperator() const
{
    return negativeOperator;
}

const TokenType Expression::getOperator(const int& index) const
{
    return operators[index];
}
