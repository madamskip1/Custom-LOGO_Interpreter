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
    std::shared_ptr<Variable> var;

    var = executeClassDeclaration(context);
    if (var)
    {
        context->addVariable(std::move(var));
        return;
    }

    var = std::make_shared<Variable>();
    var->name = identifier;
    var->type = type;

    context->addVariable(var);

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

std::shared_ptr<Variable> AST::VarDeclare::executeClassDeclaration(Context *context)
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
    if (context->evaluateValue.index() == 4)
    {
        Variable* evaluatedVar =  std::get<Variable*>(context->evaluateValue);
        if (evaluatedVar->type != TokenType::Point)
        {
            throw "class used to initalize class has to be Point type";
        }
        evaluatedVar->getSomeVal({"x"}, context);
        x = std::get<int>(context->evaluateValue);

        evaluatedVar->getSomeVal({"y"}, context);
        y = std::get<int>(context->evaluateValue);
    }
    else
    {
        x = y = std::get<int>(context->evaluateValue);

        if (classAssignment->getExpressionsSize() == 2)
        {
            classAssignment->getExpression(1)->evaluate(context);
            y = std::get<int>(context->evaluateValue);
        }
    }

    std::shared_ptr<Variable> var;

    if (type == TokenType::Turtle)
        return createTurtle(context, x, y);
    else
        return createPoint(x, y);
}

std::shared_ptr<Turtle> AST::VarDeclare::createTurtle(Context* context, std::optional<int> x, std::optional<int> y)
{
    std::shared_ptr<Turtle> turtle = std::make_shared<Turtle>(context->getDrawingBoardPtr());

    if (x)
    {
        turtle->moveTo(*x, *y);
    }

    TurtleBoard* turtleBoard = context->getTurtleBoardPtr();
    turtleBoard->addTurtle(turtle.get());

    turtle->name = identifier;
    turtle->type = type;
    return turtle;
}

std::shared_ptr<Point> AST::VarDeclare::createPoint(std::optional<int> x, std::optional<int> y)
{
    std::shared_ptr<Point> point;
    if (!x)
        point = std::make_shared<Point>();

    point = std::make_shared<Point>(*x, *y);

    point->name = identifier;
    point->type = type;
    return point;
}
