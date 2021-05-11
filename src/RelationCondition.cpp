#include "RelationCondition.h"

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

const void RelationCondition::setRelationOperator(TokenType relOperator)
{
    relationOperator = relOperator;
}

bool RelationCondition::isBooleanWord() const
{
    return isBooleanWordFlag;
}

bool RelationCondition::getBooleanWord() const
{
    return booleanWord;
}

bool RelationCondition::getNotOperator() const
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


bool RelationCondition::hasSecondExpression() const
{
    return (secondExpression != nullptr && relationOperator != TokenType::UNKNOWN);
}

TokenType RelationCondition::getRelationOperator() const
{
    return relationOperator;
}
