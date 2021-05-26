#include "AST/AssignmentStatement.h"
#include "AST/Expression.h"
#include "AST/Boolean.h"
#include "AST/Color.h"
#include "include/Context.h"
#include <QDebug>

AST::AssignmentStatement::AssignmentStatement(std::vector<std::string> ids, std::unique_ptr<AST::Assignable> assignable) : AST::Node(AST::NodeType::AssignmentStatement)
{
    identifiers = ids;
    assign = std::move(assignable);
}

void AST::AssignmentStatement::execute(Context* context)
{
    Variable* var = context->getVariable(identifiers[0]);
    if (var == nullptr)
    {
        throw "Variable wasn't declared";
    }
    std::vector<std::string> tempIdentifiers;

    if (identifiers.size() > 1)
    {
        tempIdentifiers = std::vector<std::string>(identifiers.begin() + 1, identifiers.end());
    }
    else
    {
        tempIdentifiers = std::vector<std::string>();
    }

    assign->evaluate(context);
    context->setVariant = context->evaluateValue;
    var->setSomeVal(tempIdentifiers, context);
}

std::string AST::AssignmentStatement::getIdentifier(int index) const
{
    return identifiers[index];
}

AST::Assignable* AST::AssignmentStatement::getAssign() const
{
    return assign.get();
}
