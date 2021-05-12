#include "ProgramRootNode.h"

ProgramRootNode::ProgramRootNode() : Node(NodeType::Root)
{
	instructionIterator = instructions.end();
}

std::shared_ptr<Node> ProgramRootNode::getNextInstruction()
{
	if (instructionIterator == instructions.end())
		; // Zwr�� wyj�tek lub co�
	return *instructionIterator++;
}

const void ProgramRootNode::addInstruction(std::shared_ptr<Node> instruction)
{
	instructions.push_back(instruction);
	instructionIterator = instructions.begin();
}

const bool ProgramRootNode::hasAnyInstruction() const
{
	return instructions.empty();
}

const bool ProgramRootNode::hasNextInstruction() const
{
	return (instructionIterator != instructions.end());
}

