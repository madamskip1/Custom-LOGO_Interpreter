#pragma once
#include <string>
#include <map>
#include "TokenType.h"

enum class LogType
{
	MissingElem,
	MissingSemicolon,
	NotRecognizedToken,
	MissingRoundBracketOpen,
	MissingRoundBracketClose,
	MissingCurlyBracketClose,
	MissingCurlyBracketOpen,
	MissingIdentifier,
	MissingIdentifierOrFunctionKeyword,
	CantDefFuncInBlock,
	MissingParameter,
	BadSyntaxParameter,
	BadExpression
};

class Log
{
public:
	Log(LogType logType, const int& line, const int& firstCharPos, const std::streampos& streamPos, const TokenType& type = TokenType::UNKNOWN) :
		logType(logType), line(line), firstCharPos(firstCharPos), streamPos(streamPos), type(type)
	{};

	std::string toString() const;
	LogType getLogType() const;

private:
	TokenType type = TokenType::UNKNOWN;
	int line = 0;
	int firstCharPos = 0;
	std::streampos streamPos;
	LogType logType;

	std::string lineAndPos_toString() const;

	const std::map<LogType, std::string> LogTypeToMsg =
	{
		{ LogType::MissingElem, "NULL" },
		{ LogType::MissingSemicolon, "Brakuje srednika" },
		{ LogType::NotRecognizedToken, "Nie udalo sie rozponac tokenu." },
		{ LogType::MissingRoundBracketOpen, "Oczekiwano nawiasu otwierajacego." },
		{ LogType::MissingRoundBracketClose, "Oczekiwano nawiasu zamykajacego." },
		{ LogType::MissingCurlyBracketClose, "Oczekiwano klamre zamykajaca." },
		{ LogType::MissingIdentifier, "Oczekiwano identyfikatora." },
		{ LogType::MissingIdentifierOrFunctionKeyword, "Oczekiwano identyfikatora lub slowa kluczowego \"function\"." },
		{ LogType::CantDefFuncInBlock, "Definiowanie funkcji w bloku jest niedozowlone." },
		{ LogType::MissingParameter, "Uzyto przecinka, ale nie podano kolejnego parametru." },
		{ LogType::BadSyntaxParameter, "Brakuje deklaracji typu przed nazwa parametru." },
		{ LogType::BadExpression, "Zle wyrazenie." },
		{ LogType::MissingCurlyBracketOpen, "Oczekiwano klamre otwierajaca." }
	};
};