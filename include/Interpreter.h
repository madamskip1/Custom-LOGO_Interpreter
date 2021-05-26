#pragma once
#include <memory>
#include "Context.h"
#include "drawingboard.h"
#include "turtleboard.h"
#include "../AST/ProgramRootNode.h"

class Interpreter
{
public:
    Interpreter(std::unique_ptr<AST::ProgramRootNode> rootNode, Context* context);

	void run();

private:
	std::unique_ptr<AST::ProgramRootNode> programNode;
    Context* context;

	void lookForDefFunctions();
};

