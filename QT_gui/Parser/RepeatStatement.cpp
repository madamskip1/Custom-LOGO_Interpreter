#include "RepeatStatement.h"

RepeatStatement::RepeatStatement(std::unique_ptr<Expression> howManyTime, std::unique_ptr<InstructionsBlock> block) : Node(NodeType::RepeatStatement) 
{
	howManyTimeExpression = std::move(howManyTime);
	instructionsBlock = std::move(block);
}

Expression* RepeatStatement::getHowManyTime() const
{
	return howManyTimeExpression.get();
}

InstructionsBlock* RepeatStatement::getInstructuionsBlock() const
{
	return instructionsBlock.get();
}
