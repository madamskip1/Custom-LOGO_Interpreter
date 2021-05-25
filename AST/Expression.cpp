#include "Expression.h"
#include "../include/Context.h"

AST::Expression::Expression()
{
    nodeType = NodeType::Expression;
}

int AST::Expression::evaluate(Context* context) const
{
    int val = childrenExpressions[0]->evaluate(context);
    int val2 = 0;
    TokenType op;

    for (int i = 1; i < childrenExpressions.size(); i++)
    {
        val2 = childrenExpressions[i]->evaluate(context);
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

const std::size_t AST::Expression::getChildrenExpressionSize() const
{
    return childrenExpressions.size();
}

AST::Expression* AST::Expression::getChildExpression(const int& index) const
{
    return childrenExpressions[index].get();
}

const bool AST::Expression::getNegativeOperator() const
{
    return negativeOperator;
}

const TokenType AST::Expression::getOperator(const int& index) const
{
    return operators[index];
}
