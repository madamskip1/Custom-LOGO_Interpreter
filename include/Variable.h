#pragma once
#include <string>
#include <vector>
#include "Expression.h"


class Variable :
    public Expression
{
public:
    Variable() = delete;
    Variable(std::vector<std::string> variableIdentifiers);

    std::string getIdentifier(const int& index = 0) const;

private:
    std::vector<std::string> identifiers;
};

