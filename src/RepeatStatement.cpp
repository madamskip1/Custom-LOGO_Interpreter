#include "RepeatStatement.h"

RepeatStatement::RepeatStatement() : Node(NodeType::RepeatStatement) {};

const void RepeatStatement::setHowManyTime(std::unique_ptr<Expression> howMany)
{
	howManyTime = std::move(howMany);
}

Expression* RepeatStatement::getHowManyTime() const
{
	return howManyTime.get();
}

const void RepeatStatement::setInstructionsBlock(std::unique_ptr<InstructionsBlock> block)
{
	instructionsBlock = std::move(block);
}

InstructionsBlock* RepeatStatement::getInstructuionsBlock() const
{
	return instructionsBlock.get();
}
