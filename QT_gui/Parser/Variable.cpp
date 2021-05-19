#include "Variable.h"

Variable::Variable()
{
    nodeType = NodeType::Variable;
}

const void Variable::addIdentifier(std::string identifier)
{
    identifiers.push_back(identifier);
}

const void Variable::setIdentfiers(std::vector<std::string> identfiers)
{
    this->identifiers = identfiers;
}

std::string Variable::getIdentifier(const int& index) const
{
    return identifiers[index];
}
