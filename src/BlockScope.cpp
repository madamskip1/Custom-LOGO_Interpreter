#include "BlockScope.h"

BlockScope::BlockScope()
{
}

BlockScope::~BlockScope()
{
    for (auto& var : variables)
    {
        var.second.reset();
    }

    variables.clear();
}

void BlockScope::addVariable(std::shared_ptr<Variable> var)
{
	std::string identifier = var->name;
    if (hasVariable(identifier))
	{
		throw "Variable already defined";
	}
    variables[identifier] = var;
}

void BlockScope::removeVariable(std::string identifier)
{
    if (!hasVariable(identifier))
    {
        throw "Var wasn't declared, cant delete";
    }

    variables[identifier].reset();
    variables.erase(identifier);
}

bool BlockScope::hasVariable(std::string name)
{
	if (variables.find(name) != variables.cend())
		return true;

	return false;
}

Variable* BlockScope::getVariable(std::string name)
{
    if (hasVariable(name))
	{
		return variables.at(name).get();
	}

    return nullptr;
}

std::vector<std::shared_ptr<Variable> > BlockScope::getAllVariable()
{
    std::vector<std::shared_ptr<Variable>> variablesToReturn;

    for (auto& var: variables)
    {
        variablesToReturn.push_back(var.second);
    }

    return variablesToReturn;
}
