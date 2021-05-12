#include "RelationCondition.h"

RelationCondition::RelationCondition() : Node(NodeType::RelationCondition) {};

const void RelationCondition::setBooleanWord(const bool& word)
{
    booleanWord = word;
    isBooleanWordFlag = true;
}

const void RelationCondition::setNotOperator(const bool& notOp)
{
    notOperator = notOp;
}

const void RelationCondition::setCondition(std::shared_ptr<Condition> cond)
{
    condition = cond;
}

const void RelationCondition::setExpression(std::shared_ptr<Expression> expr, const int& index)
{
    if (index != 2)
        firstExpression = expr;
    else
        secondExpression = expr;
}

const void RelationCondition::setRelationOperator(const TokenType& relOperator)
{
    relationOperator = relOperator;
}

const bool RelationCondition::isBooleanWord() const
{
    return isBooleanWordFlag;
}

const bool RelationCondition::getBooleanWord() const
{
    return booleanWord;
}

const bool RelationCondition::getNotOperator() const
{
    return notOperator;
}

std::shared_ptr<Condition> RelationCondition::getCondition() const
{
    return condition;
}

std::shared_ptr<Expression> RelationCondition::getExpression(const int& index) const
{
    if (index != 2)
        return firstExpression;

    return secondExpression;
}


const bool RelationCondition::hasSecondExpression() const
{
    return (secondExpression != nullptr && relationOperator != TokenType::UNKNOWN);
}

TokenType RelationCondition::getRelationOperator() const
{
    return relationOperator;
}
