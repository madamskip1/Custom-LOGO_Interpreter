#pragma once
#include <string>
#include "Node.h"
#include "TokenType.h"

class Parameter :
    public Node
{
public:
    Parameter();

    const void setName(const std::string paramName);
    const void setType(const TokenType type);

    std::string getName() const;
    TokenType getType() const;

private:
    std::string name;
    TokenType type = TokenType::UNKNOWN;
};

