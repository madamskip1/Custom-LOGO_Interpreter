#pragma once
#include <vector>
#include "Node.h"
#include "TokenType.h"
#include "ExpressionTerm.h"

class ExpressionTerm;

class Expression :
    public Node
{
public:
    Expression() : Node(NodeType::Expression) {};

    void addExpressionTerm(std::shared_ptr<ExpressionTerm> term);
    void addAddOperartor(TokenType addOperator);
    void addNextExpressionTerm(std::shared_ptr<ExpressionTerm> term, TokenType addOperator);

    std::size_t getTermsSize();
    std::shared_ptr<ExpressionTerm> getExpressionTerm(int index);
    TokenType getOperator(int index);

private:
    std::vector<std::shared_ptr<ExpressionTerm>> expressionTerms;
    std::vector<TokenType> addOperators;
};

