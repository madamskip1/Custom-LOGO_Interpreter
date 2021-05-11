#pragma once
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include "Log.h"
#include "Token.h"

enum class LogType
{
	MissingElem,
	MissingSemicolon,
	NotRecognizedToken,
};


class Logger
{
public:
	std::string toString() const;
	const void newLog(const LogType& logType, const Token& token);
	const void newLog(const LogType& logType, const int& line, const int& firstCharPos, const std::streampos& streamPos, const TokenType& type = TokenType::UNKNOWN);

private:
	std::vector<std::shared_ptr<Log>> logs;
};

