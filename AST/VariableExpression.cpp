#include "VariableExpression.h"
#include "../include/Context.h"
#include "../include/Variable.h"
#include "Logger.h"
#include <stdexcept>

AST::VariableExpression::VariableExpression(std::vector<std::string> variableIdentifiers)
{
    nodeType = NodeType::Variable;
    identifiers = variableIdentifiers;
}

void AST::VariableExpression::evaluate(Context *context)
{
    if (identifiers.size() == 1)
    {
        Variable* var = context->getVariable(identifiers[0]);
        if (var == nullptr)
        {
            Logger::addError(LogType::VarNotDeclared, token);
            throw std::runtime_error("Variable wasn't declared");
        }

        context->evaluateValue = std::get<int>(var->value);
    }
}

bool AST::VariableExpression::isOnlyId() const
{
    if (identifiers.size() > 0)
        return true;

    return false;
}

std::vector<std::string> AST::VariableExpression::getIdentifiers() const
{
    return identifiers;
}

std::string AST::VariableExpression::getIdentifier(const int& index) const
{
    return identifiers[index];
}
