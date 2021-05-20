#include "Token.h"

int Token::getIntValue() const
{
    return std::get<0>(value);
}

std::string Token::getStringValue() const
{
    return std::get<1>(value);
}

const bool Token::isInvalidOrUnknown()
{
    return (type == TokenType::INVALID || type == TokenType::UNKNOWN);
}

const bool Token::isType(const TokenType& typeToCheck) const
{
    return type == typeToCheck;
}

