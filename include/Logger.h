#pragma once
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include "Log.h"
#include "Token.h"

class Logger
{
public:
	std::string toString() const;
	const void newLog(const LogType& logType, const Token& token);
	const void newLog(const LogType& logType, const int& line, const int& firstCharPos, const std::streampos& streamPos, const TokenType& type = TokenType::UNKNOWN);

	const std::size_t getLogsSize() const;
	std::vector<std::shared_ptr<Log>> getLogs() const;
	std::shared_ptr<Log> getLog(int index) const;

private:
	std::vector<std::shared_ptr<Log>> logs;
};

