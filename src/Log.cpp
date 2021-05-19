#include "Log.h"

Log::Log(LogType logType, const int& line, const int& firstCharPos, const std::streampos& streamPos, const TokenType& type) :
	logType(logType), line(line), firstCharPos(firstCharPos), streamPos(streamPos), type(type)
{};

std::string Log::toString() const
{
	std::string string;
	
	if (isError())
	{
		string = "Blad krytyczny! ";
	}
	else
	{
		string = "Ostrzezenie. ";
	}

	
	string += lineAndPos_toString();	
	string += LogTypeToMsg.at(logType);

	return string;
}

LogType Log::getLogType() const
{
	return logType;
}

const void Log::setIsError(const bool& error)
{
	this->error = error;
}

const bool Log::isError() const
{
	return error;
}

std::string Log::lineAndPos_toString() const
{
	std::string string = "Linia ";
	string += std::to_string(line);
	string += ", poz ";
	string += std::to_string(firstCharPos);
	string += ": ";
	return string;
}