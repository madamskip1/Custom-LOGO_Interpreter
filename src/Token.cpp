#include "Token.h"

int Token::getIntValue() const
{
    return std::get<0>(value);
}

std::string Token::getStringValue() const
{
    return std::get<1>(value);
}

