#pragma once
#include <vector>
#include <memory>
#include "Node.h"
#include "TokenType.h"
#include "TermExpression.h"

class TermExpression;

class Expression :
    public Node
{
public:
    Expression();

    const void addExpressionTerm(std::shared_ptr<TermExpression> term);
    const void addAddOperartor(const TokenType& addOperator);
    const void addNextExpressionTerm(std::shared_ptr<TermExpression> term, const TokenType& addOperator);

    const std::size_t getTermsSize() const;
    std::shared_ptr<TermExpression> getExpressionTerm(const int& index) const;
    TokenType getOperator(const int& index) const;

private:
    std::vector<std::shared_ptr<TermExpression>> expressionTerms;
    std::vector<TokenType> addOperators;
};

