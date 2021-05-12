#include "RepeatTimeStatement.h"

RepeatTimeStatement::RepeatTimeStatement() : Node(NodeType::RepeatTimeStatement) {};

const void RepeatTimeStatement::setPeriod(std::shared_ptr<Expression> time)
{
	period = time;
}

const void RepeatTimeStatement::setHowManyTime(std::shared_ptr<Expression> howMany)
{
	howManyTime = howMany;
}

std::shared_ptr<Expression> RepeatTimeStatement::getPeriod() const
{
	return period;
}

std::shared_ptr<Expression> RepeatTimeStatement::getHowManyTime() const
{
	return howManyTime;
}

const void RepeatTimeStatement::setInstructionsBlock(std::shared_ptr<InstructionsBlock> block)
{
	instructionsBlock = block;
}

std::shared_ptr<InstructionsBlock> RepeatTimeStatement::getInstructuionsBlock() const
{
	return instructionsBlock;
}
