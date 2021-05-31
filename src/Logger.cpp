#include "Logger.h"
#include <stdexcept>

Logger* Logger::instance = nullptr;

const std::string Logger::toString()
{
    return Logger::getInstance()->makeString();
}


void Logger::addLog(const LogType &logType, const Token &token)
{
    Logger::addLog(logType, token.line, token.firstCharPos, token.tokenPos, token.type);
}

void Logger::addLog(const LogType &logType, const int &line, const int &firstCharPos, const std::streampos &streamPos, const TokenType &type)
{
    Logger::getInstance()->newLog(logType, line, firstCharPos, streamPos, type);
}

void Logger::addErrorAndThrowException(LogType logType, Token token)
{
    Logger::getInstance()->newError(logType, token);
}

bool Logger::hasAnyError()
{
    return Logger::getInstance()->anyError();
}

void Logger::clearLogs()
{
    Logger::getInstance()->clear();
}

std::string Logger::makeString() const
{
    std::string string = "";

    for (const auto& log : logs)
        string += log->toString() + "\n";

    return string;
}

void Logger::newLog(const LogType& logType, const int& line, const int& firstCharPos, const std::streampos& streamPos, const TokenType& type)
{
	std::unique_ptr<Log> newLog = std::make_unique<Log>(logType, line, firstCharPos, streamPos, type);
	logs.push_back(std::move(newLog));
}

void Logger::newError(LogType logType, const Token& token)
{
	std::unique_ptr<Log> newLog = std::make_unique<Log>(logType, token.line, token.firstCharPos, token.tokenPos, token.type);
	newLog->setIsError(true);
    std::string stringToThrow = newLog->toString();

    logs.push_back(std::move(newLog));

    throw std::runtime_error(stringToThrow.c_str());
}

Logger *Logger::getInstance()
{
    if(Logger::instance == nullptr)
    {
        Logger::instance = new Logger();
    }
    return Logger::instance;
}

const std::size_t Logger::getLogsSize() const
{
	return logs.size();
}

Log* Logger::getLog(int index) const
{
	return logs[index].get();
}

bool Logger::anyError() const
{
	int logsSize = logs.size();
	for (int i = 0; i < logsSize; i++)
	{
		if (logs[i]->isError())
			return true;
	}

	return false;
}

void Logger::clear()
{
    logs.clear();
}
