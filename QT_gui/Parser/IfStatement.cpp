#include "IfStatement.h"

IfStatement::IfStatement(std::unique_ptr<Node> condition, std::unique_ptr<InstructionsBlock> trueBlock, std::unique_ptr<InstructionsBlock> elseBlock) : Node(NodeType::IfStatement)
{
	conditionNode = std::move(condition);
	trueBlockNode = std::move(trueBlock);
	if (elseBlock)
		elseBlockNode = std::move(elseBlock);
}

const bool IfStatement::hasElseBlock() const
{
	return elseBlockNode != nullptr;
}

InstructionsBlock* IfStatement::getTrueBlockNode() const
{
	return trueBlockNode.get();
}

InstructionsBlock* IfStatement::getElseBlockNode() const
{
	return elseBlockNode.get();
}

Node* IfStatement::getCondition() const
{
	return conditionNode.get();
}
