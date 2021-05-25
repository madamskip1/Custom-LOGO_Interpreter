#include "Context.h"

Context::Context()
{
	rootScope = curScope = new BlockScope(nullptr);
}

Context::~Context()
{
    defFunctions.clear();
}

void Context::addNewScope()
{
	BlockScope* newScope = new BlockScope(curScope);

	curScope = newScope;
}

void Context::removeScope()
{
	BlockScope* temp = curScope;
	curScope = curScope->getUpperScope();
    delete temp;
}

void Context::setDrawingBoard(DrawingBoard* board)
{
    drawingBoard = board;
}

void Context::setTurtleBoard(TurtleBoard* board)
{
    turtleBoard = board;
}

DrawingBoard *Context::getDrawingBoardPtr() const
{
    return drawingBoard;
}

TurtleBoard* Context::getTurtleBoardPtr() const
{
    return turtleBoard;
}

void Context::addDefFunction(AST::DefFuncStatement* defFunction)
{
	std::string name = defFunction->getName();

	if (hasFunction(name))
		throw "function already defined";

	defFunctions[name] = defFunction;
}

void Context::addVariable(std::unique_ptr<Variable> variable)
{
	curScope->addVariable(std::move(variable));
}

Variable* Context::getVariable(std::string name)
{
	return curScope->getVariable(name);
}


AST::DefFuncStatement* Context::getDefFunction(std::string name) const
{
	return defFunctions.at(name);
}

const bool Context::hasReturn() const
{
	return (returnType != TokenType::UNKNOWN && returnVariant.index() == 0);
}

const bool Context::hasFunction(std::string name) const
{
	if (defFunctions.find(name) != defFunctions.cend())
		return true;

	return false;
}
