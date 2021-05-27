#include "BlockScope.h"

BlockScope::BlockScope(BlockScope* upper) : upperScope(upper)
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


BlockScope* BlockScope::getUpperScope() const
{
	return upperScope;
}


void BlockScope::addVariable(std::shared_ptr<Variable> var)
{
	std::string identifier = var->name;
	if (hasVariableInAnyScope(identifier))
	{
		throw "Variable already defined";
	}
    variables[identifier] = var;
}

void BlockScope::removeVariable(std::string identifier)
{
    if (hasVariableInThisScope(identifier))
    {
        variables[identifier].reset();
        variables.erase(identifier);
        return;
    }

    if (upperScope == nullptr)
        throw "Var wasn't declared, cant delete";

    upperScope->removeVariable(identifier);
}

bool BlockScope::hasVariableInThisScope(std::string name)
{
	if (variables.find(name) != variables.cend())
		return true;

	return false;
}

bool BlockScope::hasVariableInAnyScope(std::string name)
{
	if (hasVariableInThisScope(name))
		return true;

	if (upperScope != nullptr)
		return upperScope->hasVariableInAnyScope(name);

	return false;
}

Variable* BlockScope::getVariable(std::string name)
{
	if (hasVariableInThisScope(name))
	{
		return variables.at(name).get();
	}
	else if (hasVariableInAnyScope(name))
	{
		return upperScope->getVariable(name);
	}

    return nullptr;
}

std::vector<std::shared_ptr<Variable>> BlockScope::getAllVariables()
{
    std::vector<std::shared_ptr<Variable>> toReturn;
    if (upperScope != nullptr)
        toReturn = upperScope->getAllVariables();

    for (const auto& variable : variables)
    {
        toReturn.push_back(variable.second);
    }

    return toReturn;
}
