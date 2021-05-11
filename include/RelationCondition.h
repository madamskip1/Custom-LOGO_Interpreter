#pragma once
#include <vector>
#include <memory>
#include "Node.h"
#include "Condition.h"
#include "Expression.h"
#include "TokenType.h"

class Condition;

class RelationCondition :
    public Node
{
public:
    RelationCondition() : Node(NodeType::RelationCondition) {};

    const void setBooleanWord(const bool& word);
    const void setNotOperator(const bool& notOp);
    const void setCondition(std::shared_ptr<Condition> cond);
    const void setExpression(std::shared_ptr<Expression> expr, const int& index = 1);
    const void setRelationOperator(TokenType relOperator);

    bool isBooleanWord() const;
    bool getBooleanWord() const;
    bool getNotOperator() const;
    std::shared_ptr<Condition> getCondition() const;
    std::shared_ptr<Expression> getExpression(const int& index = 1) const;
    bool hasSecondExpression() const;
    TokenType getRelationOperator() const;

private:
    bool notOperator = false;
    bool isBooleanWordFlag = false;
    bool booleanWord = false;
    std::shared_ptr<Condition> condition = nullptr;

    std::shared_ptr<Expression> firstExpression = nullptr;
    TokenType relationOperator = TokenType::UNKNOWN;
    std::shared_ptr<Expression> secondExpression = nullptr;
};