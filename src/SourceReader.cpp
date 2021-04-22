#include "SourceReader.h"

const void SourceReader::setSourceString(std::string sourceString)
{
    source = std::make_unique<std::istringstream>(std::istringstream(sourceString));
    resetCounters();
}

const char SourceReader::getCharacter()
{
    char character;
    character = source.get()->get();

    if (isEndOfLine(character))
    {
        while (isEndOfLine(character))
        {
            character = source.get()->get();
        }

        lineNumber++;
        charNumber = 0;
    }
    

    
    lastCharacter = character;
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

const bool SourceReader::isEndOfLine(const char& character) const
{
    return (character == '\n' || character == '\r');
}
