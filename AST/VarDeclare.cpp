#include "VarDeclare.h"
#include "Boolean.h"
#include "Color.h"
#include "../include/Context.h"
#include "../include/Variable.h"

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
    if (type == TokenType::Turtle)
    {
        std::unique_ptr<Turtle> turtle = std::make_unique<Turtle>(context->getDrawingBoardPtr());
        if (classAssignment != nullptr)
        {
            if (classAssignment->getExpressionsSize() == 2)
            {
                int x = classAssignment->getExpression(0)->evaluate(context);
                int y = classAssignment->getExpression(1)->evaluate(context);

                turtle->move(x, y);
            }
            else
            {
                throw "wrong number of turtle pos arguments";
            }
        }

        Turtle * turtlePtr = turtle.get();
        TurtleBoard* turtleBoard = context->getTurtleBoardPtr();
        turtleBoard->addTurtle(turtlePtr);
        turtle->name = identifier;
        turtle->type = type;

        context->addVariable(std::move(turtle));
        return;
    }

    std::unique_ptr<Variable> var = std::make_unique<Variable>();
    var->name = identifier;
    var->type = type;

    context->addVariable(std::move(var));
    
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
