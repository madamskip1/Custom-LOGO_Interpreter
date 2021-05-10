#include "RepeatStatement.h"


void RepeatStatement::setHowManyTime(std::shared_ptr<Expression> howMany)
{
	howManyTime = howMany;
}

std::shared_ptr<Expression> RepeatStatement::getHowManyTime()
{
	return howManyTime;
}

const void RepeatStatement::setInstructionsBlock(std::shared_ptr<InstructionsBlock> block)
{
	instructionsBlock = block;
}

std::shared_ptr<InstructionsBlock> RepeatStatement::getInstructuionsBlock() const
{
	return instructionsBlock;
}