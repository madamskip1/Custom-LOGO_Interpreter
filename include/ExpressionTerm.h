#pragma once
#include <vector>
#include "Node.h"
#include "TokenType.h"
#include "ExpressionFactor.h"

class ExpressionFactor;

class ExpressionTerm :
    public Node
{
public:
    ExpressionTerm() : Node(NodeType::ExpressionTerm) {};

    void addExpressionFactor(std::shared_ptr<ExpressionFactor> factor);
    void addMutliOperartor(TokenType multiOperator);

private:
    std::vector<std::shared_ptr<ExpressionFactor>> expressionFactors;
    std::vector<TokenType> multiOperators;
};

