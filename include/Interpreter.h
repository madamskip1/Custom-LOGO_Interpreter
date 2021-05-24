#pragma once
#include <memory>
#include "ProgramRootNode.h"
#include "Context.h"

class Interpreter
{
public:
	Interpreter(std::unique_ptr<AST::ProgramRootNode> rootNode);

	void run();

private:
	std::unique_ptr<AST::ProgramRootNode> programNode;
	std::unique_ptr<Context> context;

	void lookForDefFunctions();
};

