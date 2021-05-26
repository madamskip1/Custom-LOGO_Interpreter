#include "SourceReader.h"
#include <fstream>
#include <sstream>


SourceReader::SourceReader() : lineNumber(0), charNumber(0), eof(false), streamPos(0), nextCharacter(NULL)
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
    else
    {
        // TODO: error, file not exist
    }
}

const char SourceReader::getCharacter()
{
    if (eof)
        return '\0';

    if (checkIfCharIsEndOfLine(getNextCharacter()))
        skipLine();

    if (eof) // skipLine can change eof flag, so check again
        return '\0';

    char character;
    character = getSingleCharFromSource();

    if (isEof())
    {
        setEof();
        return '\0';
    }

    charNumber++;
    streamPos = source.get()->tellg();

    return character;
}

const char SourceReader::getNextCharacter() 
{
    if (nextCharacter != NULL)
        return nextCharacter;

    nextCharacter = source.get()->get();

    return nextCharacter;
}

const bool SourceReader::isEof() const
{
    return source.get()->eof();
}

const void SourceReader::skipLine()
{
    // skip current line, and all nexts if empty
    char character = getSingleCharFromSource();

    while (!checkIfCharIsEndOfLine(character) && !isEof())
    {
        character = getSingleCharFromSource();
        charNumber++;
    }

    if (isEof())
    {
        setEof();
        return;
    }

    lineNumber++;
    charNumber = 0;

    char nextChar = getNextCharacter();

    if (nextChar == -1)
    {
        getSingleCharFromSource();
        setEof();
        return;
    }

    bool newNotEmptyLine = false;

    if (checkIfCharIsEndOfLine(nextChar))
    {
        if (nextChar == '\r')
        {
            getSingleCharFromSource();
            nextChar = getNextCharacter();
        }


        while (!newNotEmptyLine)
        {
            if (nextChar == '\n')
            {
                getSingleCharFromSource();
                lineNumber++;
                charNumber = 0;

                if (getNextCharacter() == '\r')
                {
                    getSingleCharFromSource();
                }

                nextChar = getNextCharacter();
            }
            else if (nextChar == -1)
            {
                getSingleCharFromSource();
                setEof();
                return;
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

const int SourceReader::getStreamPos() const
{
    return streamPos;
}

const void SourceReader::reset()
{
    lineNumber = 1;
    charNumber = 0;
    streamPos = 0;
    eof = false;
    nextCharacter = NULL;
}

const bool SourceReader::checkIfCharIsEndOfLine(const char& character) const
{
    return (character == '\n' || character == '\r');
}

const char SourceReader::getSingleCharFromSource()
{
    char temp;

    if (nextCharacter != NULL)
    {
        temp = nextCharacter;
        nextCharacter = NULL;
    }
    else
    {
        temp = source.get()->get();
    }

    return temp;
}

const void SourceReader::setEof()
{
    charNumber++;
    streamPos = source.get()->tellg();
    eof = true;
    nextCharacter = '\0';
}
