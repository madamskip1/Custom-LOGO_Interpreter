#include "RepeatStatement.h"

AST::RepeatStatement::RepeatStatement(std::unique_ptr<AST::Expression> howManyTime, std::unique_ptr<AST::InstructionsBlock> block) : AST::Node(AST::NodeType::RepeatStatement)
{
	howManyTimeExpression = std::move(howManyTime);
	instructionsBlock = std::move(block);
}

AST::Expression* AST::RepeatStatement::getHowManyTime() const
{
	return howManyTimeExpression.get();
}

AST::InstructionsBlock* AST::RepeatStatement::getInstructuionsBlock() const
{
	return instructionsBlock.get();
}
