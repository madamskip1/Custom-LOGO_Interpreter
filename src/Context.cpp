#include "Context.h"

void Context::addDefFunction(AST::DefFuncStatement* defFunction)
{
	if (isDefFunction(defFunction->getName()))
		throw "function already defined";

	defFunctions.push_back(defFunction);
}

const bool Context::isDefFunction(std::string name)
{
	for (const auto& defFunct : defFunctions)
	{
		if (defFunct->getName() == name)
			return true;
	}

	return false;
}
