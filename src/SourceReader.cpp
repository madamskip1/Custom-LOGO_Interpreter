#include "SourceReader.h"
#include <fstream>
#include <sstream>


SourceReader::SourceReader() : lineNumber(0), charNumber(0), eof(false)
{
}

const void SourceReader::setSourceString(std::string sourceString)
{
    source = std::make_unique<std::istringstream>(std::istringstream(sourceString));
    reset();
}

const void SourceReader::setSourceFile(std::string path)
{
    std::fstream file;
    file.open(path, std::ios::in);

    if (file.good())
    {
        source = std::make_unique < std::fstream>(std::move(file));
        reset();
    }
}

const char SourceReader::getCharacter()
{
    if (eof)
        return '\0';

    char character;
    character = getSingleCharFromSource();

    if (checkIfCharIsEndOfLine(character))
    {
        while (checkIfCharIsEndOfLine(character))
        {
            character = getSingleCharFromSource();
        }

        lineNumber++;
        charNumber = 0;
    }
    
    charNumber++;

    if (isEof())
    {
        eof = true;
        return '\0';
    }

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
    bool newNotEmptyLine = false;
    char character;

    character = getSingleCharFromSource();

    while (!checkIfCharIsEndOfLine(character) && !isEof())
    {
        character = getSingleCharFromSource();
        charNumber++;
    }

    if (isEof())
    {
        charNumber++;
        eof = true;
        return;
    }

    lineNumber++;
    charNumber = 0;

    char nextChar;
    nextChar = peek();

    if (checkIfCharIsEndOfLine(nextChar))
    {
        if (nextChar == '\r')
        {
            character = getSingleCharFromSource();
            nextChar = peek();
        }

        while (!newNotEmptyLine)
        {
            if (nextChar == '\n')
            {
                getSingleCharFromSource();
                lineNumber++;
                charNumber = 0;

                if (peek() == '\r')
                    getSingleCharFromSource();

                nextChar = peek();
            }
            else if (std::isspace(nextChar))
            {
                charNumber++;
                getSingleCharFromSource();
                nextChar = peek();
            }
            else
            {
                newNotEmptyLine = true;
            }
        }
    }
}

const int SourceReader::getLineNumber() const
{
    return lineNumber;
}

const int SourceReader::getCharNumber() const
{
    return charNumber;
}

const void SourceReader::reset()
{
    lineNumber = 1;
    charNumber = 0;
    streamPos = 0;
    eof = false;
}

const bool SourceReader::checkIfCharIsEndOfLine(const char& character) const
{
    return (character == '\n' || character == '\r');
}

const char SourceReader::getSingleCharFromSource()
{
    return source.get()->get();
}
