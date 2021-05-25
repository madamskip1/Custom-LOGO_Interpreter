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


    if (type == TokenType::Turtle || type == TokenType::Point)
    {
        std::optional<int> x, y = -1;

        if (classAssignment != nullptr)
        {
            if (classAssignment->getExpressionsSize() == 2)
            {
                x = classAssignment->getExpression(0)->evaluate(context);
                y = classAssignment->getExpression(1)->evaluate(context);
            }
            else if (classAssignment->getExpressionsSize() == 1)
            {
                // jedna współrzędna lub POINT
                x = classAssignment->getExpression(0)->evaluate(context);
                y = *x;
            }
            else
            {
                throw "wrong number of class arguments";
            }
        }

        if (type == TokenType::Turtle)
        {
            var = std::make_unique<Turtle>(context->getDrawingBoardPtr());
            Turtle * turtlePtr = static_cast<Turtle*>(var.get());
            if (x)
            {
                turtlePtr->move(*x, *y);
            }
            TurtleBoard* turtleBoard = context->getTurtleBoardPtr();
            turtleBoard->addTurtle(turtlePtr);
        }
        else // so its Point
        {
            if (x)
            {
                var = std::make_unique<Point>(*x, *y);
            }
            else
            {
                var = std::make_unique<Point>();
            }
        }

        var->name = identifier;
            var->type = type;

            context->addVariable(std::move(var));
            return;

    }
    var = std::make_unique<Variable>();
    var->name = identifier;
        var->type = type;

        context->addVariable(std::move(var));

    //if (type == TokenType::Integer || type == TokenType::Boolean || type == );
    if (assignment)
    {
        assignment->execute(context);
    }


    //if (type == TokenType::Integer)
    //{
    //    Expression* expression = dynamic_cast<Expression*>(assignment->getAssign());
    //    var->value = expression->evaluate();
    //    return;
    //}
    //else if (type == TokenType::Boolean)
    //{
    //    Boolean* boolean = static_cast<Boolean*>(assignment->getAssign());
    //    var->value = boolean->evaluate();
    //}
    //else if (type == TokenType::ColorVar)
    //{
    //    Color* color = static_cast<Color*>(assignment->getAssign());
    //    var->value = color->getColor();
    //}
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
