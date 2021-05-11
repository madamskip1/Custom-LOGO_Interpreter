#include "Logger.h"

std::string Logger::toString() const
{
	std::string string = "";

	for (const auto& log : logs)
		string += log->toString() + "\n";

	return string;
}

const void Logger::newLog(const LogType& logType, const Token& token)
{
	newLog(logType, token.line, token.firstCharPos, token.tokenPos, token.type);
}

const void Logger::newLog(const LogType& logType, const int& line, const int& firstCharPos, const std::streampos& streamPos, const TokenType& type)
{
	std::shared_ptr<Log> newLog = std::make_shared<Log>(logType, line, firstCharPos, streamPos, type);
	logs.push_back(newLog);
}

const std::size_t Logger::getLogsSize() const
{
	return logs.size();
}

std::vector<std::shared_ptr<Log>> Logger::getLogs() const
{
	return logs;
}

std::shared_ptr<Log> Logger::getLog(int index) const
{
	return logs[index];
}

