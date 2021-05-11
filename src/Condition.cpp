#include "Condition.h"

const void Condition::addAndCondition(std::shared_ptr<AndCondition> condition)
{
	andConditions.push_back(condition);
}

std::size_t Condition::getAndConditionSize() const
{
	return andConditions.size();;
}

std::shared_ptr<AndCondition> Condition::getAndCondition(int index) const
{
	return andConditions[index];
}
