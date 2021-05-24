#include "IfStatement.h"

AST::IfStatement::IfStatement(std::unique_ptr<AST::Node> condition, std::unique_ptr<AST::InstructionsBlock> trueBlock, std::unique_ptr<AST::InstructionsBlock> elseBlock) : AST::Node(AST::NodeType::IfStatement)
{
	conditionNode = std::move(condition);
	trueBlockNode = std::move(trueBlock);
	if (elseBlock)
		elseBlockNode = std::move(elseBlock);
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
