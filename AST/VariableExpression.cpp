#include "VariableExpression.h"
#include "../include/Context.h"
#include "../include/Variable.h"

AST::VariableExpression::VariableExpression(std::vector<std::string> variableIdentifiers)
{
    nodeType = NodeType::Variable;
    identifiers = variableIdentifiers;
}

int AST::VariableExpression::evaluate(Context *context) const
{
    if (identifiers.size() == 1)
    {
        Variable* var = context->getVariable(identifiers[0]);
        if (var == nullptr)
        {
            throw "Variable wasn't declared";
        }

        if (var->value.index() != 1)
        {
            throw "Variable in expression can be only Integer";
        }

        return std::get<int>(var->value);
    }
}

std::string AST::VariableExpression::getIdentifier(const int& index) const
{
    return identifiers[index];
}
