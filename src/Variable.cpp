#include "Variable.h"

AST::Variable::Variable(std::vector<std::string> variableIdentifiers)
{
    nodeType = NodeType::Variable;
    identifiers = variableIdentifiers;
}

std::string AST::Variable::getIdentifier(const int& index) const
{
    return identifiers[index];
}
