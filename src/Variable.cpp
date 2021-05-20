#include "Variable.h"

Variable::Variable(std::vector<std::string> variableIdentifiers)
{
    nodeType = NodeType::Variable;
    identifiers = variableIdentifiers;
}

std::string Variable::getIdentifier(const int& index) const
{
    return identifiers[index];
}
