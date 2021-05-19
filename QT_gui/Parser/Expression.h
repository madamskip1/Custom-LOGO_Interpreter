#pragma once
#include <vector>
#include <memory>
#include "TokenType.h"
#include "Node.h"
#include "Assignable.h"

class Expression :
    public Assignable
{
public:
    Expression();
    virtual int evaluate() const;

    const void addChildExpression(std::unique_ptr<Expression> child);
    const void addOperator(const TokenType& op);
    const void setNegativeOp(const bool& negative);
    const std::size_t getChildrenExpressionSize() const;

    Expression* getChildExpression(const int& index) const;
    const bool getNegativeOperator() const;
    const TokenType getOperator(const int& index) const;

protected:
    bool negativeOperator = false;
    std::vector<std::unique_ptr<Expression>> childrenExpressions;
    std::vector<TokenType> operators;
};

