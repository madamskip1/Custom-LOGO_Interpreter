#include "IfStatement.h"

IfStatement::IfStatement() : Node(NodeType::IfStatement) {};

const void IfStatement::setTrueBlockNode(std::shared_ptr<InstructionsBlock> block)
{
	trueBlockNode = block;
}

const void IfStatement::setElseBlockNode(std::shared_ptr<InstructionsBlock> block)
{
	elseBlockNode = block;
}

const void IfStatement::setCondition(std::shared_ptr<Condition> cond)
{
	condition = cond;
}

const bool IfStatement::hasElseBlock() const
{
	return elseBlockNode != nullptr;
}

std::shared_ptr<InstructionsBlock> IfStatement::getTrueBlockNode() const
{
	return trueBlockNode;
}

std::shared_ptr<InstructionsBlock> IfStatement::getElseBlockNode() const
{
	return elseBlockNode;
}

std::shared_ptr<Condition> IfStatement::getCondition() const
{
	return condition;
}
