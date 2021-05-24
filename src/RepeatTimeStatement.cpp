#include "RepeatTimeStatement.h"

AST::RepeatTimeStatement::RepeatTimeStatement(std::unique_ptr<AST::Expression> period, std::unique_ptr<AST::InstructionsBlock> block) : AST::Node(AST::NodeType::RepeatTimeStatement)
{
	periodExpression = std::move(period);
	instructionsBlock = std::move(block);
	howManyTimeExpression = nullptr;
}

AST::RepeatTimeStatement::RepeatTimeStatement(std::unique_ptr<AST::Expression> period, std::unique_ptr<AST::Expression> howManyTime, std::unique_ptr<AST::InstructionsBlock> block) : AST::Node(AST::NodeType::RepeatTimeStatement)
{
	periodExpression = std::move(period);
	howManyTimeExpression = std::move(howManyTime);
	instructionsBlock = std::move(block);
}


AST::Expression* AST::RepeatTimeStatement::getPeriod() const
{
	return periodExpression.get();
}

AST::Expression* AST::RepeatTimeStatement::getHowManyTime() const
{
	return howManyTimeExpression.get();
}

AST::InstructionsBlock* AST::RepeatTimeStatement::getInstructuionsBlock() const
{
	return instructionsBlock.get();
}
