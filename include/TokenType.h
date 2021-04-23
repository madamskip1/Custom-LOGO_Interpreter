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
	String,
	Integer,
	Turtle,
	Point,
	Boolean,

	// Values
	StringVal,
	Digit,

	// Others
	AssignOperator,
	NotOperator,
	Identifier,
	EndOfFile,
	UNKNOWN,
	INVALID
};

const std::array<char, 4> mathOperators = { '+', '-', '/', '*' };

const std::map<char, TokenType> MathOperatorsToTokenType =
{
	{ '+', TokenType::Plus },
	{ '-', TokenType::Minus },
	{ '/', TokenType::Divide },
	{ '*', TokenType::Multiply }
};

const std::map<std::string, TokenType> KeywordToTokenType =
{
	{ "repeat", TokenType::Repeat },
	{ "repeatTime", TokenType::RepeatTime },
	{ "if", TokenType::If },
	{ "else", TokenType::Else },
	{ "function", TokenType::Function },
	{ "return", TokenType::Return },
	{ "Integer", TokenType::Integer },
	{ "String", TokenType::String },
	{ "Turtle", TokenType::Turtle },
	{ "Point", TokenType::Point },
	{ "Boolean", TokenType::Boolean },
	{ "true", TokenType::True },
	{ "false", TokenType::False },
};