#pragma once
#include "TokenType.h"
#include <string>
#include <variant>

class Token {
public:
	int getIntValue() const;
	std::string getStringValue() const;
	const bool isInvalidOrUnknown();

	const bool isType(const TokenType& typeToCheck) const;


	TokenType type = TokenType::UNKNOWN;
	std::variant<int, std::string> value;

	int line = 0;
	int firstCharPos = 0;
	std::streampos tokenPos;
};