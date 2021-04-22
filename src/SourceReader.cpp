#include "SourceReader.h"
#include <fstream>
#include <sstream>


SourceReader::SourceReader() : lineNumber(0), charNumber(0)
{
}

const void SourceReader::setSourceString(std::string sourceString)
{
    source = std::make_unique<std::istringstream>(std::istringstream(sourceString));
    resetCounters();
}

const void SourceReader::setSourceFile(std::string path)
{
    std::fstream file;
    file.open(path, std::ios::in);

    if (file.good())
    {
        source = std::make_unique < std::fstream>(std::move(file));
        resetCounters();
    }
}

const char SourceReader::getCharacter()
{
    char character;
    character = source.get()->get();

    if (checkIfCharIsEndOfLine(character))
    {
        while (checkIfCharIsEndOfLine(character))
        {
            character = source.get()->get();
        }

        lineNumber++;
        charNumber = 0;
    }
    
    charNumber++;

    return character;
}

const char SourceReader::peek() const
{
    return source.get()->peek();
}

const bool SourceReader::isEof() const
{
    return source.get()->eof();
}

const void SourceReader::skipLine()
{
    std::string temp;
    std::getline(*(source.get()), temp);
}

const int SourceReader::getLineNumber() const
{
    return lineNumber;
}

const int SourceReader::getCharNumber() const
{
    return charNumber;
}

const void SourceReader::resetCounters()
{
    lineNumber = 1;
    charNumber = 0;
}

const bool SourceReader::checkIfCharIsEndOfLine(const char& character) const
{
    return (character == '\n' || character == '\r');
}
