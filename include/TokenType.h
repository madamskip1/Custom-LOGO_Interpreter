#pragma once
#include <map>
#include <array>
#include <string>

enum class TokenType : unsigned int
{
	// Keywords
	Repeat,
	RepeatTime,
	Delete,
	Function,
	If,
	Else,
	Return,

	// Brackets
	CurlyBracketOpen,
	CurlyBracketClose,
	RoundBracketOpen,
	RoundBracketClose,

	// Math Operator
	Plus,
	Minus,
	Multiply,
	Divide,

	// Symbols
	Comma,
	Dot,
	Semicolon,

	// Relation Operators
	Equal,
	NotEqual,
	Less,
	Greater,
	LessOrEqual,
	GreaterOrEqual,

	// Condition Operator
	And,
	Or,

	// Boolean values
	True,
	False,

	// Data Types
	ColorVar,
	Integer,
	Turtle,
	Point,
	Boolean,

	// Values
	ColorValue,
	Digit,

	// Others
	AssignOperator,
	NotOperator,
	Identifier,
	EndOfFile,


	// Errors token
	BadDigitZeros,
	BadDigitTooLong,

	ColorValMissHash,
	ColorValBadSyntax,
	ColorValTooLong,
	ColorValTooShort,
	ColorValNotTerminated,

	AndOperatorMissSecond,
	OrOperatorMissSecond,

	UNKNOWN,
	INVALID
};

const std::array<char, 4> mathOperators = { '+', '-', '/', '*' };

static const std::map<char, TokenType> MathOperatorsToTokenType =
{
	{ '+', TokenType::Plus },
	{ '-', TokenType::Minus },
	{ '/', TokenType::Divide },
	{ '*', TokenType::Multiply }
};

static const std::map<std::string, TokenType> KeywordToTokenType =
{
	{ "repeat", TokenType::Repeat },
	{ "repeatTime", TokenType::RepeatTime },
	{ "if", TokenType::If },
	{ "else", TokenType::Else },
	{ "function", TokenType::Function },
	{ "return", TokenType::Return },
	{ "Integer", TokenType::Integer },
	{ "Color", TokenType::ColorVar },
	{ "Turtle", TokenType::Turtle },
	{ "Point", TokenType::Point },
	{ "Boolean", TokenType::Boolean },
	{ "true", TokenType::True },
	{ "false", TokenType::False },
};
