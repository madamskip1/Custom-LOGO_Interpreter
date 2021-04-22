#pragma once
#include "TokenType.h"
#include <string>
#include <variant>

struct Token {
	TokenType type = TokenType::UNKNOWN;
	std::variant<int, bool, std::string> value;

	int line = 0;
	int firstCharPos = 0;
};