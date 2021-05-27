#include "Expression.h"
#include "../include/Context.h"

AST::Expression::Expression()
{
    nodeType = NodeType::Expression;
}

void AST::Expression::evaluate(Context* context)
{
    if (isOnlyId())
    {
        std::vector<std::string> identifiers = getIdentifiers();
        Variable* var = context->getVariable(identifiers[0]);
        identifiers.erase(identifiers.begin());
        var->getSomeVal(identifiers, context);
        return;
    }

    childrenExpressions[0]->evaluate(context);
    int val = std::get<int>(context->evaluateValue);

    int val2 = 0;
    TokenType op;

    for (std::size_t i = 1; i < childrenExpressions.size(); i++)
    {
        childrenExpressions[i]->evaluate(context);
        val2 = std::get<int>(context->evaluateValue);

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

    context->evaluateValue = val;
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
