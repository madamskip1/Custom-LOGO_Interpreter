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
    x = y = std::nullopt;

    if (classAssignment->getExpressionsSize() == 1)
    {
        AST::Expression* expression = classAssignment->getExpression(0);
        if (expression->isOnlyId(context))
        {
            std::vector<std::string> identifiers = expression->getIdentifiers(context);
            if (identifiers.size() == 1)
            {
                Variable* var = context->getVariable(identifiers[0]);
                TokenType varType = var->type;

                if (varType == TokenType::Point)
                {
                    Point* point = static_cast<Point*>(var);
                    point->getSomeVal(std::vector<std::string>({"x"}), context);
                    x = std::get<int>(context->returnVariant);
                    point->getSomeVal(std::vector<std::string>({"y"}), context);
                    y = std::get<int>(context->returnVariant);
                    context->returnVariant = std::monostate{};
                }
                else
                {
                    Turtle* point = static_cast<Turtle*>(var);
                    point->getSomeVal(std::vector<std::string>({"pos", "x"}), context);
                    x = std::get<int>(context->returnVariant);
                    point->getSomeVal(std::vector<std::string>({"pos", "y"}), context);
                    y = std::get<int>(context->returnVariant);
                    context->returnVariant = std::monostate{};
                }
            }
        }
    }

    if (!x)
    {
        x = y = resolveClassExpression(context, 0);

        if (classAssignment->getExpressionsSize() == 2)
        {
            y = resolveClassExpression(context, 1);
        }
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
        if(!y)
            y = *x;

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

int AST::VarDeclare::resolveClassExpression(Context* context, int index)
{
    AST::Expression* expression = classAssignment->getExpression(index);

    if (!expression->isOnlyId(context))
    {
        return expression->evaluate(context);
    }

    std::vector<std::string> identifiers = classAssignment->getExpression(index)->getIdentifiers(context);

    Variable* var = context->getVariable(identifiers[0]);
    TokenType varType = var->type;

    if (varType == TokenType::Point)
    {
        Point* point = static_cast<Point*>(var);
        point->getSomeVal(std::vector<std::string>(identifiers.begin() + 1, identifiers.end()), context);

        return std::get<int>(context->returnVariant);
    }
    else if (varType == TokenType::Turtle)
    {
        Turtle* point = static_cast<Turtle*>(var);
        point->getSomeVal(std::vector<std::string>(identifiers.begin() + 1, identifiers.end()), context);
        return std::get<int>(context->returnVariant);
    }
    else if (identifiers.size() == 1)
    {
        return expression->evaluate(context);
    }

    throw "cant resolve var class expression";
}

