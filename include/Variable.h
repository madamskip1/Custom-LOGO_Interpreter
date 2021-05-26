#pragma once
#include <variant>
#include <string>
#include <vector>
#include "TokenType.h"

class Context;

class Variable
{
public:
    virtual ~Variable() {};

    std::variant<std::monostate, int, std::string, bool, Variable*> value;
	std::string name;
	TokenType type;

    virtual void getSomeVal(std::vector<std::string> /* identifiers */, Context* context);
    virtual void setSomeVal(std::vector<std::string> /* identifiers */, Context* context);
};

