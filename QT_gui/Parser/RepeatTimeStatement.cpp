#include "RepeatTimeStatement.h"

RepeatTimeStatement::RepeatTimeStatement() : Node(NodeType::RepeatTimeStatement) {};

const void RepeatTimeStatement::setPeriod(std::unique_ptr<Expression> time)
{
	period = std::move(time);
}

const void RepeatTimeStatement::setHowManyTime(std::unique_ptr<Expression> howMany)
{
	howManyTime = std::move(howMany);
}

Expression* RepeatTimeStatement::getPeriod() const
{
	return period.get();
}

Expression* RepeatTimeStatement::getHowManyTime() const
{
	return howManyTime.get();
}

const void RepeatTimeStatement::setInstructionsBlock(std::unique_ptr<InstructionsBlock> block)
{
	instructionsBlock = std::move(block);
}

InstructionsBlock* RepeatTimeStatement::getInstructuionsBlock() const
{
	return instructionsBlock.get();
}
