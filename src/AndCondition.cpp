#include "AndCondition.h"

const void AndCondition::addRelationCondition(std::shared_ptr<RelationCondition> condition)
{
    relationConditions.push_back(condition);
}

int AndCondition::getRelationConditionSize() const
{
    return relationConditions.size();
}

std::shared_ptr<RelationCondition> AndCondition::getRelationCondition(int index) const
{
    return relationConditions[index];
}
