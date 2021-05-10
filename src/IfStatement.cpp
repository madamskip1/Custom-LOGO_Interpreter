#include "IfStatement.h"

void IfStatement::setTrueBlockNode(std::shared_ptr<InstructionsBlock> block)
{
	trueBlockNode = block;
}

void IfStatement::setElseBlockNode(std::shared_ptr<InstructionsBlock> block)
{
	elseBlockNode = block;
}

std::shared_ptr<InstructionsBlock> IfStatement::getTrueBlockNode()
{
	return trueBlockNode;
}

std::shared_ptr<InstructionsBlock> IfStatement::getElseBlockNode()
{
	return elseBlockNode;
}
