#pragma once
#include <string>
#include "Node.h"
#include "TokenType.h"

class Parameter :
    public Node
{
public:
    Parameter() = delete;
    Parameter(TokenType parameterType, std::string parameterName);

    std::string getName() const;
    TokenType getType() const;

private:
    std::string name;
    TokenType type = TokenType::UNKNOWN;
};

