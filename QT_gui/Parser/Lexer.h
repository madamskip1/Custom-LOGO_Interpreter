#pragma once
#include <map>
#include <string>
#include "Token.h"
#include "SourceReader.h"

class Lexer
{
public:
	Lexer() = delete;
	Lexer(SourceReader* source);

	Token getNextToken();

private:
	SourceReader* source;
	Token curToken;
	char firstCharacter;

	const bool tryToMakeDigit();
	const bool tryToMakeColor();
	const bool tryToMakeIDorKeywordOrDatatypes();
	const bool tryToMakeMathOperator();
	const bool tryToMakeConditionOperator();
	const bool tryToMakeRelationOperator();
	const bool tryToMakeBracket();
	const bool tryToMakeSymbols();

	const bool isComment() const;

	const static std::map<std::string, int> lexerConfig;
};