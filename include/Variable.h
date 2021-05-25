#pragma once
#include <variant>
#include <string>
#include "TokenType.h"

class Variable
{
public:
	std::variant<std::monostate, int, bool, std::string> value;
	std::string name;
	TokenType type;
};

