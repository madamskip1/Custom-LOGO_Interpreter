#include "Lexer.h"
#include <iostream>
#include <stdexcept>
#include "Logger.h"


const std::unordered_map<std::string, int> Lexer::lexerConfig =
{
	{ "maxNumOfDigits", 9 }
};

Lexer::Lexer(SourceReader& reader) : source(reader), firstCharacter(-1)
{
}

Token Lexer::getNextToken()
{
	curToken.type = TokenType::INVALID;
	firstCharacter = source.getCharacter();

	while (std::isspace(firstCharacter) || isComment())
		firstCharacter = source.getCharacter();


	curToken.line = source.getLineNumber();
	curToken.firstCharPos = source.getCharNumber();
	curToken.tokenPos = source.getStreamPos();

	if (source.isEof())
	{
		curToken.type = TokenType::EndOfFile;
		return curToken;
	}

	if (tryToMakeDigit())
		return curToken;

	if (tryToMakeColor())
		return curToken;

	if (tryToMakeIDorKeywordOrDatatypes())
		return curToken;

	if (tryToMakeMathOperator())
		return curToken;

	if (tryToMakeConditionOperator())
		return curToken;

	if (tryToMakeRelationOperator())
		return curToken;

	if (tryToMakeBracket())
		return curToken;

	tryToMakeSymbols();

	return curToken;
}


const bool Lexer::tryToMakeDigit()
{
	if (!std::isdigit(firstCharacter))
		return false;

	int val = firstCharacter - '0';
	curToken.type = TokenType::Digit;

	if (val == 0)
    {
        char nextCharTemp = source.getNextCharacter();
        if (nextCharTemp >= '0' || nextCharTemp <= '9')
		{
			source.getCharacter();
            curToken.type = TokenType::INVALID;

            Logger::addErrorAndThrowException(LogType::BadDigitZeros, curToken);
		}
	}
	else
	{
		int counter = 1;
		char digit;

		while (std::isdigit(source.getNextCharacter()))
		{
			digit = source.getCharacter();
			counter++;
			
			if (counter > Lexer::lexerConfig.at("maxNumOfDigits"))
			{
                curToken.type = TokenType::INVALID;

                Logger::addErrorAndThrowException(LogType::BadDigitTooLong, curToken);
			}

            val = val * 10 + (digit - '0');
		}
	}

	curToken.value = val;

	return true;
}

const bool Lexer::tryToMakeColor()
{
	if (firstCharacter != '"')
		return false;


	char nextChar = source.getNextCharacter();
	char curCharacter;
	std::string string = "";

	int counter = 0;
	int expectedSize = 7;
	bool badSyntax = false;
	
	while (std::isprint(nextChar) && nextChar != '"' && !source.isEof())
	{
		counter++;
		curCharacter = source.getCharacter();

		if (badSyntax || 
			(counter != 1 && (curCharacter < '0' || curCharacter > 'F' || (curCharacter > '59' && curCharacter < '65'))) ||
			(counter == 1 && curCharacter != '#'))
		{
			badSyntax = true;
		}
		else
		{
			string += curCharacter;
		}

		nextChar = source.getNextCharacter();
	}

	if (nextChar != '"')
	{
        curToken.type = TokenType::INVALID;

        Logger::addErrorAndThrowException(LogType::ColorValNotTerminated, curToken);
	}

	source.getCharacter();

	if (counter != expectedSize)
	{
		if (counter < expectedSize)
		{
            curToken.type = TokenType::INVALID;

            Logger::addErrorAndThrowException(LogType::ColorValTooShort, curToken);
		}
		else
		{
            curToken.type = TokenType::INVALID;

            Logger::addErrorAndThrowException(LogType::ColorValTooLong, curToken);
		}
	}
	else if (string.length() < 1 || string.at(0) != '#')
	{
        curToken.type = TokenType::INVALID;

        Logger::addErrorAndThrowException(LogType::ColorValMissHash, curToken);
	}
	else if (badSyntax)
	{
        curToken.type = TokenType::INVALID;

        Logger::addErrorAndThrowException(LogType::ColorValBadSyntax, curToken);
	}
	else
	{
		curToken.type = TokenType::ColorValue;
		curToken.value = string;
	}

	return true;
}

const bool Lexer::tryToMakeIDorKeywordOrDatatypes()
{
	if (!std::isalpha(firstCharacter))
		return false;
	
	std::string name(1, firstCharacter);

	char nextChar = source.getNextCharacter();

	while (std::isalnum(nextChar) || nextChar == '_')
	{
		name += source.getCharacter();
		nextChar = source.getNextCharacter();
	}

    std::unordered_map<std::string, TokenType>::const_iterator keywordIterator = KeywordToTokenType.find(name);

	if (keywordIterator != KeywordToTokenType.end())
	{
		curToken.type = std::get<1>(*keywordIterator);
	}
	else
	{
		curToken.type = TokenType::Identifier;
		curToken.value = name;
	}

	return true;
}

const bool Lexer::tryToMakeMathOperator()
{
	std::array<char, 4>::const_iterator end = std::cend(mathOperators);

	if (std::find(std::begin(mathOperators), end, firstCharacter) == end)
	{
		return false;
	}

	if (firstCharacter == '/' && source.getNextCharacter() == '/')
	{
		return false;
	}

    std::unordered_map<char, TokenType>::const_iterator operatorType = MathOperatorsToTokenType.find(firstCharacter);

	curToken.type = std::get<1>(*operatorType);

	return true;
}

const bool Lexer::tryToMakeConditionOperator()
{
	if (firstCharacter == '&')
	{
		if (source.getNextCharacter() != '&')
		{
            Logger::addErrorAndThrowException(LogType::AndOperatorMissSecond, curToken);
		}

		source.getCharacter();
		curToken.type = TokenType::And;
		return true;
	}
	else if (firstCharacter == '|')
	{
		if (source.getNextCharacter() != '|')
		{
            Logger::addErrorAndThrowException(LogType::OrOperatorMissSecond, curToken);
		}

		source.getCharacter();
		curToken.type = TokenType::Or;
		return true;
	}

	return false;
}

const bool Lexer::tryToMakeRelationOperator()
{
	if (firstCharacter == '<')
	{
		if (source.getNextCharacter() == '=')
		{
			source.getCharacter();
			curToken.type = TokenType::LessOrEqual;
			return true;
		}

		curToken.type = TokenType::Less;
		return true;
	}
	else if (firstCharacter == '>')
	{
		if (source.getNextCharacter() == '=')
		{
			source.getCharacter();
			curToken.type = TokenType::GreaterOrEqual;
			return true;
		}

		curToken.type = TokenType::Greater;
		return true;
	}
	else if (firstCharacter == '=')
	{
		if (source.getNextCharacter() == '=')
		{
			source.getCharacter();
			curToken.type = TokenType::Equal;
			return true;
		}

		curToken.type = TokenType::AssignOperator;
		return true;
	}
	else if (firstCharacter == '!')
	{
		if (source.getNextCharacter() == '=')
		{
			source.getCharacter();
			curToken.type = TokenType::NotEqual;
			return true;
		}

		curToken.type = TokenType::NotOperator;
		return true;
	}

	return false;
}

const bool Lexer::tryToMakeBracket()
{
	if (firstCharacter == '{')
	{
		curToken.type = TokenType::CurlyBracketOpen;
		return true;
	}
	else if (firstCharacter == '}')
	{
		curToken.type = TokenType::CurlyBracketClose;
		return true;
	}
	else if (firstCharacter == '(')
	{
		curToken.type = TokenType::RoundBracketOpen;
		return true;
	}
	else if (firstCharacter == ')')
	{
		curToken.type = TokenType::RoundBracketClose;
		return true;
	}

	return false;
}

const bool Lexer::tryToMakeSymbols()
{
	if (firstCharacter == '.')
	{
		curToken.type = TokenType::Dot;
		return true;
	}
	else if (firstCharacter == ';')
	{
		curToken.type = TokenType::Semicolon;
		return true;
	}
	else if (firstCharacter == ',')
	{
		curToken.type = TokenType::Comma;
		return true;
	}

	return false;
}

const bool Lexer::isComment() const
{
	if (firstCharacter != '/' || source.getNextCharacter() != '/')
		return false;

	source.skipLine();
	return true;
}

