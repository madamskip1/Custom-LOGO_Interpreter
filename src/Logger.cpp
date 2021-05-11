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
	std::shared_ptr<Log> newLog;
	if (logType == LogType::NotRecognizedToken)
	{
		newLog = std::make_shared<NotRecognizedToken>(line, firstCharPos, streamPos, type);
	}
	else if (logType == LogType::MissingSemicolon)
	{
		newLog = std::make_shared<MissingSemicolon>(line, firstCharPos, streamPos, type);
	}

	
	logs.push_back(newLog);
}

