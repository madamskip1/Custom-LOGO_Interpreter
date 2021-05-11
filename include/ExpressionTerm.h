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
    void addNextExpressionFactor(std::shared_ptr<ExpressionFactor> factor, TokenType multiOperator);

    std::size_t getFactorsSize();
    std::shared_ptr<ExpressionFactor> getExpressionFactor(int index);
    TokenType getOperator(int index);

private:
    std::vector<std::shared_ptr<ExpressionFactor>> expressionFactors;
    std::vector<TokenType> multiOperators;
};

