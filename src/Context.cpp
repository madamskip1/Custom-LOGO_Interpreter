#include <stdexcept>
#include "../include/Context.h"
#include "TurtleStdFunc.h"
#include "Logger.h"


Context::Context()
{
    addNewScope();
    prepareStdLibFunctions();
}


void Context::addNewScope()
{
    scopes.push_back(BlockScope());
}

void Context::removeScope()
{
    if (scopes.size() <= 1)
        return;

    scopes.pop_back();
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
        std::runtime_error("function already defined");

	defFunctions[name] = defFunction;
}

void Context::addVariable(std::shared_ptr<Variable> variable)
{
    if (args.find(variable->name) != args.cend())
    {
        throw std::runtime_error("cant declare var with same name as parameter");
    }

    if (hasVariable(variable->name))
    {
        Logger::addErrorAndThrowException(LogType::VarAlreadyDeclared, Token());
    }

    scopes.back().addVariable(variable);
}

void Context::removeVariable(std::string identifier)
{
    for (auto& scope : scopes)
    {
        if (scope.hasVariable(identifier))
        {
            scope.removeVariable(identifier);
            return;
        }
    }
}

Variable* Context::getVariable(std::string name)
{
    for (auto& scope : scopes)
    {
        if (scope.hasVariable(name))
        {
            return scope.getVariable(name);
        }
    }

    if (args.find(name) != args.end())
    {
        return args[name];
    }

    return nullptr;
}

bool Context::hasVariable(std::string name)
{
    for (auto& scope : scopes)
    {
        if (scope.hasVariable(name))
        {
            return true;
        }
    }

    return false;
}

std::vector<std::shared_ptr<Variable>> Context::getAllCurrentVariables()
{
    std::vector<std::shared_ptr<Variable>> variables;

    for (auto& scope : scopes)
    {
        std::vector<std::shared_ptr<Variable>> scopeVariables = scope.getAllVariable();
        variables.insert(variables.end(), scopeVariables.begin(), scopeVariables.end());
    }

    return variables;
}

AST::DefFuncStatement* Context::getDefFunction(std::string name) const
{
    return defFunctions.at(name);
}

std::function<void (Context *)> Context::getStdLibFunction(std::string name) const
{
    return stdLibFunctions.at(name);
}

bool Context::hasReturn() const
{
    return (returnType != TokenType::UNKNOWN && returnVariant.index() == 0);
}

void Context::prepareStdLibFunctions()
{
    stdLibFunctions.emplace("clean", clean);
    stdLibFunctions.emplace("allToStart", allToStart);
}

bool Context::hasFunction(std::string name) const
{
	if (defFunctions.find(name) != defFunctions.cend())
		return true;

    return false;
}

bool Context::hasStdLibFunction(std::string name) const
{
    if (stdLibFunctions.find(name) != stdLibFunctions.cend())
        return true;

    return false;
}
