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
	std::unique_ptr<Log> newLog = std::make_unique<Log>(logType, line, firstCharPos, streamPos, type);
	logs.push_back(std::move(newLog));
}

const void Logger::addNewError(LogType logType, const Token& token)
{
	std::unique_ptr<Log> newLog = std::make_unique<Log>(logType, token.line, token.firstCharPos, token.tokenPos, token.type);
	newLog->setIsError(true);
	logs.push_back(std::move(newLog));
}

const std::size_t Logger::getLogsSize() const
{
	return logs.size();
}

Log* Logger::getLog(int index) const
{
	return logs[index].get();
}

const bool Logger::hasAnyError() const
{
	int logsSize = logs.size();
	for (int i = 0; i < logsSize; i++)
	{
		if (logs[i]->isError())
			return true;
	}

	return false;
}

