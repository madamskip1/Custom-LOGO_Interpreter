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
}

std::string AST::AssignmentStatement::getIdentifier(int index) const
{
    return identifiers[index];
}


AST::Assignable* AST::AssignmentStatement::getAssign() const
{
    return assign.get();
}
