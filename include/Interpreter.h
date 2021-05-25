#pragma once
#include <memory>
#include "Context.h"
#include "drawingboard.h"
#include "turtleboard.h"
#include "../AST/ProgramRootNode.h"

class Interpreter
{
public:
    Interpreter(std::unique_ptr<AST::ProgramRootNode> rootNode, DrawingBoard* drawingBoard = nullptr, TurtleBoard* turtleBoard = nullptr);

	void run();

private:
	std::unique_ptr<AST::ProgramRootNode> programNode;
	std::unique_ptr<Context> context;

	void lookForDefFunctions();
};

