#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Log.h"
#include "Token.h"

class Logger
{
public:
	std::string toString() const;

	const void newLog(const LogType& logType, const Token& token);
	const void newLog(const LogType& logType, const int& line, const int& firstCharPos, const std::streampos& streamPos, const TokenType& type = TokenType::UNKNOWN);
	const void addNewError(LogType logType, const Token& token);

	const std::size_t getLogsSize() const;
	Log* getLog(int index) const;

	const bool hasAnyError() const;

private:
	std::vector<std::unique_ptr<Log>> logs;
};

