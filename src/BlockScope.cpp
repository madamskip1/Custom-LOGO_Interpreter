#include "BlockScope.h"

BlockScope::BlockScope(BlockScope* upper) : upperScope(upper)
{
}


BlockScope* BlockScope::getUpperScope() const
{
	return upperScope;
}


void BlockScope::addVariable(std::unique_ptr<Variable> var)
{
	std::string identifier = var->name;
	if (hasVariableInAnyScope(identifier))
	{
		throw "Variable already defined";
	}
	variables[identifier] = std::move(var);
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
