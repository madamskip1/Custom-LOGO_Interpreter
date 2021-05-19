#pragma once
#include <vector>
#include <memory>
#include "Node.h"

class ProgramRootNode :
	public Node
{
public:
	ProgramRootNode();
	
	const void addInstruction(std::shared_ptr<Node> instruction);

	std::shared_ptr<Node> getNextInstruction();
	const bool hasAnyInstruction() const;
	const bool hasNextInstruction() const;

private:
	std::vector<std::shared_ptr<Node>>::iterator instructionIterator;
	std::vector<std::shared_ptr<Node>> instructions;
};

