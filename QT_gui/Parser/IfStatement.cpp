#include "IfStatement.h"

IfStatement::IfStatement() : Node(NodeType::IfStatement) {};

const void IfStatement::setTrueBlockNode(std::unique_ptr<InstructionsBlock> block)
{
	trueBlockNode = std::move(block);
}

const void IfStatement::setElseBlockNode(std::unique_ptr<InstructionsBlock> block)
{
	elseBlockNode = std::move(block);
}

const void IfStatement::setCondition(std::unique_ptr<Node> cond)
{
	condition = std::move(cond);
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
	return condition.get();
}
