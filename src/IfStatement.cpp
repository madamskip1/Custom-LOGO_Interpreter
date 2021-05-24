#include "IfStatement.h"
#include <iostream>

AST::IfStatement::IfStatement(std::unique_ptr<AST::Node> condition, std::unique_ptr<AST::InstructionsBlock> trueBlock, std::unique_ptr<AST::InstructionsBlock> elseBlock) : AST::Node(AST::NodeType::IfStatement)
{
	conditionNode = std::move(condition);
	trueBlockNode = std::move(trueBlock);
	if (elseBlock)
		elseBlockNode = std::move(elseBlock);
}

void AST::IfStatement::execute(Context* context)
{
	bool ifCondition = (static_cast<Condition*>(conditionNode.get()))->evaluate();

	if (ifCondition)
	{
		std::cout << "TRUE BLOCK" << std::endl;
		trueBlockNode->execute(context);
	}
	else if (elseBlockNode)
	{
		std::cout << "ELSE BLOCK" << std::endl;
		elseBlockNode->execute(context);
	}
}


const bool AST::IfStatement::hasElseBlock() const
{
	return elseBlockNode != nullptr;
}

AST::InstructionsBlock* AST::IfStatement::getTrueBlockNode() const
{
	return trueBlockNode.get();
}

AST::InstructionsBlock* AST::IfStatement::getElseBlockNode() const
{
	return elseBlockNode.get();
}

AST::Node* AST::IfStatement::getCondition() const
{
	return conditionNode.get();
}
