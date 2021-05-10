#include "ProgramRootNode.h"

ProgramRootNode::ProgramRootNode()
{
	instructionIterator = instructions.end();
}

std::shared_ptr<Node> ProgramRootNode::getNextInstruction()
{
	if (instructionIterator == instructions.end())
		; // Zwróæ wyj¹tek lub coœ
	return *instructionIterator++;
}

void ProgramRootNode::addInstruction(std::shared_ptr<Node> instruction)
{
	instructions.push_back(instruction);
	instructionIterator = instructions.begin();
}

bool ProgramRootNode::hasAnyInstruction()
{
	return instructions.empty();
}

bool ProgramRootNode::hasNextInstruction()
{
	return (instructionIterator != instructions.end());
}

