#pragma once
#include <string>
#include "TokenType.h"


class Log
{
public:
	Log(const int& line, const int& firstCharPos, const std::streampos& streamPos, const TokenType& type = TokenType::UNKNOWN) :
		line(line), firstCharPos(firstCharPos), streamPos(streamPos), type(type)
	{};

	virtual std::string toString() const = 0;

protected:
	TokenType type = TokenType::UNKNOWN;
	int line = 0;
	int firstCharPos = 0;
	std::streampos streamPos;
};

class NotRecognizedToken :
	public Log
{
	using Log::Log;

public:
	virtual std::string toString() const;
};

class MissingSemicolon :
	public Log
{
	using Log::Log;

public:
	virtual std::string toString() const;
};

//
//class Missing :
//	public Log
//{
//	using Log::Log;
//};