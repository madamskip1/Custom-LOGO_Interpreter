#pragma once
#include <vector>
#include <memory>
#include "Node.h"

class ProgramRootNode
{
public:
	ProgramRootNode();

	std::shared_ptr<Node> getNextInstruction();
	void addInstruction(std::shared_ptr<Node> instruction);

	bool hasAnyInstruction();
	bool hasNextInstruction();

private:
	std::vector<std::shared_ptr<Node>>::iterator instructionIterator;
	std::vector<std::shared_ptr<Node>> instructions;
};

