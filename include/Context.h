#pragma once
#include <map>
#include <string>
#include <variant>
#include "../AST/Node.h"
#include "../AST/DefFuncStatement.h"
#include "BlockScope.h"
#include "Variable.h"
#include "turtleboard.h"
#include "drawingboard.h"

class Context
{
public:
	std::variant<std::monostate, int, std::string, /*Turtle, Color*/ bool> returnVariant;
	TokenType returnType = TokenType::UNKNOWN;

	Context();
    ~Context();

	void addNewScope();
	void removeScope();

    void setDrawingBoard(DrawingBoard* board);
    void setTurtleBoard(TurtleBoard* board);
    DrawingBoard* getDrawingBoardPtr() const;
    TurtleBoard* getTurtleBoardPtr() const;

	void addDefFunction(AST::DefFuncStatement* defFunction);
	void addVariable(std::unique_ptr<Variable> variable);
	Variable* getVariable(std::string name);

	AST::DefFuncStatement* getDefFunction(std::string name) const;

	const bool hasFunction(std::string name) const;
	const bool hasReturn() const;
private:
	std::map<std::string, AST::DefFuncStatement*> defFunctions;
	BlockScope* rootScope; // Global
	BlockScope* curScope;
	
    DrawingBoard* drawingBoard;
    TurtleBoard* turtleBoard;
};

