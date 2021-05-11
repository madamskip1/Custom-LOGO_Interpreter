#pragma once
#include <vector>
#include "Node.h"
#include "RelationCondition.h"

class RelationCondition;

class AndCondition :
    public Node
{
public:
    AndCondition() : Node(NodeType::AndCondition) {};

    const void addRelationCondition(std::shared_ptr<RelationCondition> condition);

    int getRelationConditionSize() const;
    std::shared_ptr<RelationCondition> getRelationCondition(int index = 0) const;

private:
    std::vector<std::shared_ptr<RelationCondition>> relationConditions;
};

