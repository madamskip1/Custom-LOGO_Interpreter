#pragma once
#include <vector>
#include <memory>
#include "Node.h"
#include "RelationCondition.h"

class RelationCondition;

class AndCondition :
    public Node
{
public:
    AndCondition();

    const void addRelationCondition(std::shared_ptr<RelationCondition> condition);

    std::size_t getRelationConditionSize() const;
    std::shared_ptr<RelationCondition> getRelationCondition(int index = 0) const;

private:
    std::vector<std::shared_ptr<RelationCondition>> relationConditions;
};

