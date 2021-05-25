#include "RepeatStatement.h"
#include <iostream>


AST::RepeatStatement::RepeatStatement(std::unique_ptr<AST::Expression> howManyTime, std::unique_ptr<AST::InstructionsBlock> block) : AST::Node(AST::NodeType::RepeatStatement)
{
	howManyTimeExpression = std::move(howManyTime);
	instructionsBlock = std::move(block);
}

void AST::RepeatStatement::execute(Context* context)
{
    int repeat = howManyTimeExpression->evaluate(context);

	for (int i = 0; i < repeat; i++)
	{
		instructionsBlock->execute(context);
	}
}

AST::Expression* AST::RepeatStatement::getHowManyTime() const
{
	return howManyTimeExpression.get();
}

AST::InstructionsBlock* AST::RepeatStatement::getInstructuionsBlock() const
{
	return instructionsBlock.get();
}
