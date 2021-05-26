#include "AST/AssignmentStatement.h"
#include "AST/Expression.h"
#include "AST/Boolean.h"
#include "AST/Color.h"
#include "include/Context.h"

AST::AssignmentStatement::AssignmentStatement(std::vector<std::string> ids, std::unique_ptr<AST::Assignable> assignable) : AST::Node(AST::NodeType::AssignmentStatement)
{
    identifiers = ids;
    assign = std::move(assignable);
}

void AST::AssignmentStatement::execute(Context* context)
{
    if (identifiers.size() == 1)
    {
        Variable* var = context->getVariable(identifiers[0]);
        if (var == nullptr)
        {
            throw "Variable wasn't declared";
        }
        AST::Assignable* assignable = assign.get();

        if (assignable->getNodeType() == AST::NodeType::Expression)
        {
            Expression* expression = dynamic_cast<Expression*>(assignable);
            int value = expression->evaluate(context);
            var->value = value;
            return;
        }
        else if (assignable->getNodeType() == AST::NodeType::Boolean)
        {
            Boolean* boolean = static_cast<Boolean*>(assignable);
            var->value = boolean->evaluate();
        }
        else if (assignable->getNodeType() == AST::NodeType::Color)
        {
            Color* color = static_cast<Color*>(assignable);
            var->value = color->getColor();
        }
    }
    else if (identifiers.size() > 1)
    {
        Variable* var = context->getVariable(identifiers[0]);
        if (var->type == TokenType::Point)
        {
            Expression* expression = dynamic_cast<Expression*>(assign.get());
            int value = expression->evaluate(context);
            context->setVariant = value;
            Point* point = static_cast<Point*>(var);
            point->setSomeVal(std::vector<std::string>(identifiers.begin() + 1, identifiers.end()), context);
            return;
        }
        else if (var->type == TokenType::Turtle)
        {
            Turtle* turtle = static_cast<Turtle*>(var);

            AST::Assignable* assignable = assign.get();
            if (assignable->getNodeType() == AST::NodeType::Expression)
            {
                Expression* expression = dynamic_cast<Expression*>(assignable);
                int value = expression->evaluate(context);
                context->setVariant = value;
            }
            else if (assignable->getNodeType() == AST::NodeType::Boolean)
            {
                Boolean* boolean = static_cast<Boolean*>(assignable);
                context->setVariant = boolean->evaluate();
            }
            else if (assignable->getNodeType() == AST::NodeType::Color)
            {
                Color* color = static_cast<Color*>(assignable);
                context->setVariant = color->getColor();
            }

            turtle->setSomeVal(std::vector<std::string>(identifiers.begin() + 1, identifiers.end()), context);
        }
    }
}

std::string AST::AssignmentStatement::getIdentifier(int index) const
{
    return identifiers[index];
}


AST::Assignable* AST::AssignmentStatement::getAssign() const
{
    return assign.get();
}
