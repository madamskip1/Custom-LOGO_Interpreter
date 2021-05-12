#include "Condition.h"

Condition::Condition() : Node(NodeType::Condition) {};

const void Condition::addAndCondition(std::shared_ptr<AndCondition> condition)
{
	andConditions.push_back(condition);
}

const std::size_t Condition::getAndConditionSize() const
{
	return andConditions.size();;
}

std::shared_ptr<AndCondition> Condition::getAndCondition(const int& index) const
{
	return andConditions[index];
}
