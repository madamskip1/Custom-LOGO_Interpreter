#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Log.h"
#include "Token.h"

class Logger
{
public:
    static const std::string toString();

    static void addLog(const LogType& logType, const Token& token);
    static void addLog(const LogType& logType, const int& line, const int& firstCharPos, const std::streampos& streamPos, const TokenType& type = TokenType::UNKNOWN);
    static void addErrorAndThrowException(LogType logType, Token token);
    static bool hasAnyError();
    static void clearLogs();

    static Logger* getInstance();

	const std::size_t getLogsSize() const;
	Log* getLog(int index) const;

private:
    Logger() {};
    static Logger* instance;
	std::vector<std::unique_ptr<Log>> logs;

    void newLog(const LogType& logType, const int& line, const int& firstCharPos, const std::streampos& streamPos, const TokenType& type = TokenType::UNKNOWN);
    void newError(LogType logType, const Token& token);
    std::string makeString() const;
    bool anyError() const;
    void clear();
};

