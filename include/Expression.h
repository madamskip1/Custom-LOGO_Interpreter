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

private:
    std::vector<std::shared_ptr<ExpressionTerm>> expressionTerms;
    std::vector<TokenType> addOperators;
};

