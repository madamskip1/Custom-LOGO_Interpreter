#pragma once
#include <map>
#include <string>
#include <memory>
#include "Variable.h"

class BlockScope
{
public:
	std::map<std::string, std::unique_ptr<Variable>> variables;
	
	BlockScope(BlockScope* upper);

	BlockScope* getUpperScope() const;

	void addVariable(std::unique_ptr<Variable> var);
	bool hasVariableInThisScope(std::string name);
	bool hasVariableInAnyScope(std::string name);

	Variable* getVariable(std::string name);

private:
	BlockScope* upperScope = nullptr;
};

