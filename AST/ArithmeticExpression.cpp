#include "ArithmeticExpression.h"
#include "../include/Context.h"

AST::ArithmeticExpression::ArithmeticExpression()
{
    nodeType = NodeType::ArithmeticExpression;
}

void AST::ArithmeticExpression::evaluate(Context* context)
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

    if (context->evaluateValue.index() != 1)
        return;

    int val = std::get<int>(context->evaluateValue);

    for (std::size_t i = 1; i < childrenExpressions.size(); i++)
    {
        childrenExpressions[i]->evaluate(context);
        int val2 = std::get<int>(context->evaluateValue);

        TokenType op = operators[i - 1];

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

bool AST::ArithmeticExpression::isOnlyId() const
{
    if (childrenExpressions.size() != 1)
        return false;

    return childrenExpressions[0]->isOnlyId();
}

std::vector<std::string> AST::ArithmeticExpression::getIdentifiers() const
{
    if (childrenExpressions.size() != 1)
        throw "expression isn't just identifiers";

    return childrenExpressions[0]->getIdentifiers();
}


const void AST::ArithmeticExpression::addChildExpression(std::unique_ptr<AST::Expression> child)
{
    childrenExpressions.push_back(std::move(child));
}

const void AST::ArithmeticExpression::addOperator(const TokenType& op)
{
    operators.push_back(op);
}

const void AST::ArithmeticExpression::setNegativeOp(const bool& negative)
{
    negativeOperator = negative;
}

const std::size_t AST::ArithmeticExpression::getChildrenExpressionSize() const
{
    return childrenExpressions.size();
}

AST::Expression* AST::ArithmeticExpression::getChildExpression(const int& index) const
{
    return childrenExpressions[index].get();
}

const bool AST::ArithmeticExpression::getNegativeOperator() const
{
    return negativeOperator;
}

const TokenType AST::ArithmeticExpression::getOperator(const int& index) const
{
    return operators[index];
}
