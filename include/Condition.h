#pragma once
#include <vector>
#include <memory>
#include "Node.h"
#include "AndCondition.h"

class AndCondition;

class Condition :
    public Node
{
public:
    Condition();

    const void addAndCondition(std::shared_ptr<AndCondition> condition);

    const std::size_t getAndConditionSize() const;
    std::shared_ptr<AndCondition> getAndCondition(const int& index = 0) const;

private:
    std::vector<std::shared_ptr<AndCondition>> andConditions;
};