#pragma once
#include <memory>
#include "Node.h"
#include "TokenType.h"

class Condition :
    public Node
{
public:
    Condition();

    virtual bool evaluate();
   
    const void setLeftCondition(std::unique_ptr<Node> condition);
    const void setRightCondition(std::unique_ptr<Node> condition);
    const void setNotOperator(const bool& notOp);
    const void setRelationOperator(const TokenType& relOp);

    Node* getLeftCondition() const;
    Node* getRightCondition() const;
    const bool getNotOperator() const;
    const TokenType getRelationOperator() const;

private:
    std::unique_ptr<Node> leftCondition = nullptr;
    std::unique_ptr<Node> rightCondition = nullptr;
    TokenType relationOperator;
    bool notOperator;
};

