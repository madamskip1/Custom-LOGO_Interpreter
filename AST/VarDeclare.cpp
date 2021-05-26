#include "VarDeclare.h"
#include "Boolean.h"
#include "Color.h"
#include "../include/Context.h"
#include "../include/Variable.h"
#include "../include/Point.h"

AST::VarDeclare::VarDeclare(TokenType type, std::string identifier) : Node(AST::NodeType::VarDeclare)
{
    this->type = type;
    this->identifier = identifier;
    classAssignment = nullptr;
    assignment = nullptr;
}

AST::VarDeclare::VarDeclare(TokenType type, std::string identifier, std::unique_ptr<AssignmentStatement> assignStatement) : AST::Node(AST::NodeType::VarDeclare)
{
    this->type = type;
    this->identifier = identifier;
    assignment = std::move(assignStatement);
    classAssignment = nullptr;
}

AST::VarDeclare::VarDeclare(TokenType type, std::string identifier, std::unique_ptr<ClassAssignment> classAssign) : AST::Node(AST::NodeType::VarDeclare)
{
    this->type = type;
    this->identifier = identifier;
    classAssignment = std::move(classAssign);
    assignment = nullptr;
}

void AST::VarDeclare::execute(Context* context)
{
    std::unique_ptr<Variable> var;

    var = executeClassDeclaration(context);
    if (var)
    {
        context->addVariable(std::move(var));
        return;
    }

    var = std::make_unique<Variable>();
    var->name = identifier;
    var->type = type;

    context->addVariable(std::move(var));

    if (assignment)
    {
        assignment->execute(context);
    }
}

TokenType AST::VarDeclare::getType() const
{
    return type;
}

std::string AST::VarDeclare::getIdentifier() const
{
    return identifier;
}

AST::ClassAssignment* AST::VarDeclare::getClassAssignment() const
{
    return classAssignment.get();
}

AST::AssignmentStatement* AST::VarDeclare::getAssignment() const
{
    return assignment.get();
}

std::unique_ptr<Variable> AST::VarDeclare::executeClassDeclaration(Context *context)
{
    if (type != TokenType::Turtle && type != TokenType::Point)
        return nullptr;

    if (!classAssignment)
    {
        if (type == TokenType::Turtle)
            return createTurtle(context);
        else
            return createPoint();
    }

    if (classAssignment->getExpressionsSize() < 1 && classAssignment->getExpressionsSize() > 2)
        throw "wrong number of class arguments";

    std::optional<int> x, y;

    classAssignment->getExpression(0)->evaluate(context);
    x = y = std::get<int>(context->evaluateValues[0]);

    if (context->evaluateValues.size() == 2)
    {
        y = std::get<int>(context->evaluateValues[1]);
    }
    else if (classAssignment->getExpressionsSize() == 2)
    {
        classAssignment->getExpression(1)->evaluate(context);
        y = std::get<int>(context->evaluateValues[0]);
    }


    if (type == TokenType::Turtle)
        return createTurtle(context, x, y);
    else
        return createPoint(x, y);
}

std::unique_ptr<Turtle> AST::VarDeclare::createTurtle(Context* context, std::optional<int> x, std::optional<int> y)
{
    std::unique_ptr<Turtle> turtle = std::make_unique<Turtle>(context->getDrawingBoardPtr());
    turtle->name = identifier;
    turtle->type = type;
    if (x)
    {
        turtle->move(*x, *y);
    }

    TurtleBoard* turtleBoard = context->getTurtleBoardPtr();
    turtleBoard->addTurtle(turtle.get());

    return turtle;
}

std::unique_ptr<Point> AST::VarDeclare::createPoint(std::optional<int> x, std::optional<int> y)
{
    std::unique_ptr<Point> point;
    if (!x)
        point = std::make_unique<Point>();

    point = std::make_unique<Point>(*x, *y);

    point->name = identifier;
    point->type = type;

    return point;
}
