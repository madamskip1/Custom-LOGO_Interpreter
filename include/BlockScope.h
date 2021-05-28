#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "Variable.h"

class BlockScope
{
public:	
    BlockScope();
    ~BlockScope();

    void addVariable(std::shared_ptr<Variable> var);
    void removeVariable(std::string identifier);

    Variable* getVariable(std::string name);
    std::vector<std::shared_ptr<Variable>> getAllVariable();

    bool hasVariable(std::string name);

private:
    std::unordered_map<std::string, std::shared_ptr<Variable>> variables;
};

