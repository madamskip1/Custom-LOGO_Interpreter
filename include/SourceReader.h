#pragma once
#include <istream>
#include <sstream>
#include <string>

class SourceReader
{
public:
	const void setSourceString(std::string sourceString);
	const char getCharacter();
	const char peek() const;
	const bool isEof() const;
	const void skipLine();

	const int getLineNumber() const;
	const int getCharNumber() const;

private:
	std::unique_ptr<std::istream> source;
	char lastCharacter;
	int lineNumber;
	int charNumber;

	const void resetCounters();
	const bool isEndOfLine(const char& character) const;
};

