#pragma once
#include <string>
#include "Token.h"
#include "SourceReader.h"

class Lexer
{
public:
	std::string characters;
	Lexer(SourceReader* source);

	Token getNextToken();

private:
	SourceReader* source;
	Token curToken;

	const void tryToMakeDigit(const char& character);
	const void tryToMakeString(const char& character);
	const void tryToMakeIDorKeywordOrDatatypes(const char& character);
	const void tryToMakeMathOperator(const char& character);
	const void tryToMakeConditionOperator(const char& character);
	const void tryToMakeRelationOperator(const char& character);
	const void tryToMakeBracket(const char& character);
	const void tryToMakeSymbols(const char& character);
	const void tryToMakeComment(const char& character);
};