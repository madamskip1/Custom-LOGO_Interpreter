#pragma once
#include <variant>
#include <string>
#include <vector>
#include "TokenType.h"
#include "Token.h"

class Context;

class Variable
{
public:
    virtual ~Variable() {};

    std::variant<std::monostate, int, std::string, bool, Variable*> value;
    std::string name;
    TokenType type;
    Token* token;

    virtual void getSomeVal(std::vector<std::string> /* identifiers */, Context* context);
    virtual void setSomeVal(std::vector<std::string> /* identifiers */, Context* context);
};

