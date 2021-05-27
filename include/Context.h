#pragma once
#include <unordered_map>
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
    std::variant<std::monostate, int, std::string, bool, Variable*> setVariant;
    std::variant<std::monostate, int, std::string, bool, Variable*> evaluateValue;
    std::unordered_map<std::string, Variable*> args;

    std::variant<std::monostate, int, std::string, bool, Variable*> returnVariant;
    TokenType returnType = TokenType::UNKNOWN;


    Context();

    void addNewScope();
    void removeScope();

    void setDrawingBoard(DrawingBoard* board);
    void setTurtleBoard(TurtleBoard* board);
    DrawingBoard* getDrawingBoardPtr() const;
    TurtleBoard* getTurtleBoardPtr() const;

	void addDefFunction(AST::DefFuncStatement* defFunction);
	void addVariable(std::unique_ptr<Variable> variable);
    AST::DefFuncStatement* getDefFunction(std::string name) const;
    std::function<void(Context*)> getStdLibFunction(std::string name) const;

    Variable* getVariable(std::string name);

    bool hasFunction(std::string name) const;
    bool hasStdLibFunction(std::string name) const;
    bool hasReturn() const;

private:
    std::unordered_map<std::string, AST::DefFuncStatement*> defFunctions;
    std::unordered_map<std::string, std::function<void(Context*)>> stdLibFunctions;
    BlockScope* rootScope; // Global scope
    BlockScope* curScope;
	
    DrawingBoard* drawingBoard;
    TurtleBoard* turtleBoard;

    void prepareStdLibFunctions();
};

