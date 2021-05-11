#include "Log.h"

std::string Log::toString() const
{
	std::string string = lineAndPos_toString();
	
	string += LogTypeToMsg.at(logType);

	return string;
}

LogType Log::getLogType() const
{
	return logType;
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