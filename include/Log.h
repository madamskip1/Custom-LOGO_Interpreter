#pragma once
#include <string>
#include <unordered_map>
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
	BadExpression,
	BadSyntax,
	NotEndOfFile,
	BadReturnStatement,
	UnknownAssignable,
    BadCondition,
    DeleteMissingIdentifier,

    // execute/evaluate/interpreter
    VarNotDeclared,
    VarAlreadyDeclared,
    FuncNotDefined,
    NotEqualeNumArgs,
    ArgTypeNotEqualParameter,
    WrongClassArgsNum,
    InitClassNotPointType,
    DivideByZero,

    // Lexer
    BadDigitZeros,
    BadDigitTooLong,
    ColorValNotTerminated,
    ColorValTooShort,
    ColorValTooLong,
    ColorValMissHash,
    ColorValBadSyntax,
    AndOperatorMissSecond,
    OrOperatorMissSecond
};

class Log
{
public:
	Log(LogType logType, const int& line, const int& firstCharPos, const std::streampos& streamPos, const TokenType& type = TokenType::UNKNOWN);

	std::string toString() const;
	LogType getLogType() const;

	const void setIsError(const bool& error);
	const bool isError() const;

private:
	TokenType type = TokenType::UNKNOWN;
	int line = 0;
	int firstCharPos = 0;
	std::streampos streamPos;
	LogType logType;

	bool error = false;
	
	std::string lineAndPos_toString() const;

    const std::unordered_map<LogType, std::string> LogTypeToMsg =
	{
		{ LogType::MissingElem, "NULL" },
        { LogType::MissingSemicolon, "Brakuje średnika" },
        { LogType::NotRecognizedToken, "Nie udało sie rozponać tokenu." },
        { LogType::MissingRoundBracketOpen, "Oczekiwano nawiasu otwierającego." },
        { LogType::MissingRoundBracketClose, "Oczekiwano nawiasu zamykającego." },
        { LogType::MissingCurlyBracketClose, "Oczekiwano klamrę zamykającą." },
		{ LogType::MissingIdentifier, "Oczekiwano identyfikatora." },
        { LogType::MissingIdentifierOrFunctionKeyword, "Oczekiwano identyfikatora lub słowa kluczowego \"function\"." },
		{ LogType::CantDefFuncInBlock, "Definiowanie funkcji w bloku jest niedozowlone." },
        { LogType::MissingParameter, "Użyto przecinka, ale nie podano kolejnego parametru." },
        { LogType::BadSyntaxParameter, "Brakuje deklaracji typu przed nazwą parametru." },
        { LogType::BadExpression, "Złe wyrażenie." },
        { LogType::MissingCurlyBracketOpen, "Oczekiwano klamrę otwierającą." },
        { LogType::BadSyntax, "Bład składniowy." },
        { LogType::NotEndOfFile, "Nie udało sie przetworzyć do końca." },
        { LogType::BadReturnStatement, "Błąd składniowy w klauzyli Return" },
        { LogType::UnknownAssignable, "Nie rozpoznanu rodzaju przypisania wartości" },
        { LogType::BadCondition, "Złe wyrażenie warunkowe" },
        { LogType::DeleteMissingIdentifier, "Brakuje identyfikatora przy delete" },
        { LogType::VarNotDeclared, "Zmienna nie została zadeklarowana" },
        { LogType::VarAlreadyDeclared, "Zmienna jest już zadeklarowana" },
        { LogType::FuncNotDefined, "Funkcja nie została zdefiniowana" },
        { LogType::NotEqualeNumArgs, "Podano złą liczbę argumentów funkcji" },
        { LogType::ArgTypeNotEqualParameter, "Argument ma inny typ niż zdefiniowany parametr" },
        { LogType::WrongClassArgsNum, "Deklaracja klasy przyjmuje jeden lub dwa argumenty" },
        { LogType::InitClassNotPointType, "Jedyną klasą, którą można inicjalizować inne klasy jest POINT" },
        { LogType::BadDigitZeros, "Zera poprzedzające liczbę" },
        { LogType::BadDigitTooLong, "Liczba jest zbyt duża" },
        { LogType::ColorValNotTerminated, "Brakuje zamkyjącego cudzysłowia w wartości koloru" },
        { LogType::ColorValTooShort, "Wartość koloru jest zbyt krótka" },
        { LogType::ColorValTooLong, "Wartość koloru jest zbyt długa" },
        { LogType::ColorValMissHash, "Wartość koloru nie zaczyna się od \"#\" "},
        { LogType::ColorValBadSyntax, "Kolor zawiera nieprawidłowe znaki" },
        { LogType::AndOperatorMissSecond, "W operatorze koniunkcji AND brakuje drugiego znaku &" },
        { LogType::OrOperatorMissSecond, "W operatorze alterantywy OR brakuje drugiego znaku |" },
        { LogType::DivideByZero, "Nie można dzielić przez zero." }
    };
};
