#pragma once
#include <map>
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

	const bool tryToMakeDigit(const char& character);
	const bool tryToMakeString(const char& character);
	const bool tryToMakeIDorKeywordOrDatatypes(const char& character);
	const bool tryToMakeMathOperator(const char& character);
	const bool tryToMakeConditionOperator(const char& character);
	const bool tryToMakeRelationOperator(const char& character);
	const bool tryToMakeBracket(const char& character);
	const bool tryToMakeSymbols(const char& character);

	const bool isComment(const char& character);

	const static std::map<std::string, int> lexerConfig;
};