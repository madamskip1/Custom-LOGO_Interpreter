#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Variable.h"

class BlockScope
{
public:	
    BlockScope(BlockScope* upper);
    ~BlockScope();

    void addVariable(std::shared_ptr<Variable> var);
    void removeVariable(std::string identifier);

    Variable* getVariable(std::string name);
    std::vector<std::shared_ptr<Variable>> getAllVariables();
    BlockScope* getUpperScope() const;

    bool hasVariableInThisScope(std::string name);
    bool hasVariableInAnyScope(std::string name);

private:
    std::unordered_map<std::string, std::shared_ptr<Variable>> variables;
	BlockScope* upperScope = nullptr;
};

