#include "RepeatTimeStatement.h"

RepeatTimeStatement::RepeatTimeStatement(std::unique_ptr<Expression> period, std::unique_ptr<InstructionsBlock> block) : Node(NodeType::RepeatTimeStatement)
{
	periodExpression = std::move(period);
	instructionsBlock = std::move(block);
	howManyTimeExpression = nullptr;
}

RepeatTimeStatement::RepeatTimeStatement(std::unique_ptr<Expression> period, std::unique_ptr<Expression> howManyTime, std::unique_ptr<InstructionsBlock> block) : Node(NodeType::RepeatTimeStatement)
{
	periodExpression = std::move(period);
	howManyTimeExpression = std::move(howManyTime);
	instructionsBlock = std::move(block);
}


Expression* RepeatTimeStatement::getPeriod() const
{
	return periodExpression.get();
}

Expression* RepeatTimeStatement::getHowManyTime() const
{
	return howManyTimeExpression.get();
}

InstructionsBlock* RepeatTimeStatement::getInstructuionsBlock() const
{
	return instructionsBlock.get();
}
