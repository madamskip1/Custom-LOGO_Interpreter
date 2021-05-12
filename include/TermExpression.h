#pragma once
#include <vector>
#include <memory>
#include "Node.h"
#include "TokenType.h"
#include "FactorExpression.h"

class FactorExpression;

class TermExpression :
    public Node
{
public:
    TermExpression();

    const void addExpressionFactor(std::shared_ptr<FactorExpression> factor);
    const void addMutliOperartor(const TokenType& multiOperator);
    const void addNextExpressionFactor(std::shared_ptr<FactorExpression> factor, const TokenType& multiOperator);

    const std::size_t getFactorsSize() const;
    std::shared_ptr<FactorExpression> getExpressionFactor(const int& index) const;
    TokenType getOperator(const int& index) const;

private:
    std::vector<std::shared_ptr<FactorExpression>> expressionFactors;
    std::vector<TokenType> multiOperators;
};

