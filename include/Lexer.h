#pragma once
#include <string>
#include "Token.h"
#include "SourceReader.h"


class Lexer
{
public:
	std::string characters;
	Lexer(SourceReader* source);
	Token nextToken();
private:
	SourceReader* source;
	Token token;

	const void tryToMakeDigit(const char& character);
	const void tryToMakeString(const char& character);
	const void tryToMakeIDorKeywordOrDatatypes(const char character);
	const void tryToMakeMathOperator(const char& character);
	const void tryToMakeConditionOperator(const char& character);
	const void tryToRelationOperator(const char& character);
	const void tryToMakeBracket(const char& character);
	const void tryToMakeSymbols(const char& character);
	const void tryToMakeComment(const char& character);
};

