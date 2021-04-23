#pragma once
#include <istream>
#include <string>

class SourceReader
{
public:
	SourceReader();
	const void setSourceString(std::string sourceString);
	const void setSourceFile(std::string path);
	const char getCharacter();
	const char peek() const;
	const bool isEof() const;
	const void skipLine();

	const int getLineNumber() const;
	const int getCharNumber() const;

private:
	std::unique_ptr<std::istream> source;
	int lineNumber;
	int charNumber;
	bool eof;
	std::streampos streamPos;

	const void reset();
	const bool checkIfCharIsEndOfLine(const char& character) const;
	const char getSingleCharFromSource();
};

