#pragma once
#include <vector>
#include "Node.h"
#include "AndCondition.h"

class AndCondition;

class Condition :
    public Node
{
public:
    Condition() : Node(NodeType::Condition) {};

    const void addAndCondition(std::shared_ptr<AndCondition> condition);

    std::size_t getAndConditionSize() const;
    std::shared_ptr<AndCondition> getAndCondition(int index = 0) const;

private:
    std::vector<std::shared_ptr<AndCondition>> andConditions;
};