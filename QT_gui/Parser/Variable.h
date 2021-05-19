#pragma once
#include <string>
#include <vector>
#include "Expression.h"


class Variable :
    public Expression
{
public:
    Variable();
    
    const void addIdentifier(std::string identifier);
    const void setIdentfiers(std::vector<std::string> identfiers);
    std::string getIdentifier(const int& index = 0) const;

private:
    std::vector<std::string> identifiers;
};

