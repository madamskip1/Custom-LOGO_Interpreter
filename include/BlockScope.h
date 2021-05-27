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

    void addVariable(std::unique_ptr<Variable> var);
    void removeVariable(std::string identifier);

    Variable* getVariable(std::string name);
    BlockScope* getUpperScope() const;

    bool hasVariableInThisScope(std::string name);
    bool hasVariableInAnyScope(std::string name);

private:
    std::unordered_map<std::string, std::unique_ptr<Variable>> variables;
	BlockScope* upperScope = nullptr;
};

