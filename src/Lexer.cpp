#include "Lexer.h"
#include <iostream>

const std::map<std::string, int> Lexer::lexerConfig =
{
	{ "maxNumOfDigits", 9 }
};

Lexer::Lexer(SourceReader* reader) : source(reader)
{
}

Token Lexer::getNextToken()
{
	curToken.type = TokenType::INVALID;
	char character;
	character = source->getCharacter();

	while (std::isspace(character))
		character = source->getCharacter();

	while (isComment(character))
		character = source->getCharacter();

	curToken.line = source->getLineNumber();
	curToken.firstCharPos = source->getCharNumber();
	curToken.tokenPos = source->getStreamPos();

	if (source->isEof())
	{
		curToken.type = TokenType::EndOfFile;
		return curToken;
	}

	if (tryToMakeDigit(character))
		return curToken;

	if (tryToMakeString(character))
		return curToken;

	if (tryToMakeIDorKeywordOrDatatypes(character))
		return curToken;

	if (tryToMakeMathOperator(character))
		return curToken;

	if (tryToMakeConditionOperator(character))
		return curToken;

	if (tryToMakeRelationOperator(character))
		return curToken;

	if (tryToMakeBracket(character))
		return curToken;

	if (tryToMakeSymbols(character))
		return curToken;

	return curToken;
}

const bool Lexer::tryToMakeDigit(const char& character)
{
	if (!std::isdigit(character))
		return false;

	int val = character - '0';
	curToken.type = TokenType::Digit;

	if (val == 0)
	{
		if ((source->peek() - '0') == 0)
		{
			source->getCharacter();
			curToken.type = TokenType::BadDigitZeros;

			// now skip all digits
			while (std::isdigit(source->peek()))
				source->getCharacter();

			return true;
		}
	}
	else
	{
		int counter = 1;
		char digit;

		while (std::isdigit(source->peek()))
		{
			digit = source->getCharacter();
			counter++;
			
			if (counter > Lexer::lexerConfig.at("maxNumOfDigits"))
			{
				curToken.type = TokenType::BadDigitTooLong;
				// now skip all digits
			}
			else
			{
				val = val * 10 + (digit - '0');
			}


		}
	}

	curToken.value = val;

	return true;
}

const bool Lexer::tryToMakeString(const char& character)
{
	if (character != '"')
		return false;

	char nextChar = source->peek();
	std::string string = "";

	while (std::isprint(nextChar) && nextChar != '"' && !source->isEof())
	{
		string += source->getCharacter();
		nextChar = source->peek();
	}

	if (nextChar != '"')
	{
		curToken.type = TokenType::INVALID;
		return true;
	}

	source->getCharacter();

	curToken.type = TokenType::StringVal;
	curToken.value = string;

	return true;
}

const bool Lexer::tryToMakeIDorKeywordOrDatatypes(const char& character)
{
	if (!std::isalpha(character))
		return false;
	
	std::string name(1, character);

	char nextChar = source->peek();

	while (std::isalnum(nextChar) || nextChar == '_')
	{
		name += source->getCharacter();
		nextChar = source->peek();
	}

	std::map<std::string, TokenType>::const_iterator keywordIterator = KeywordToTokenType.find(name);

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

const bool Lexer::tryToMakeMathOperator(const char& character)
{
	std::array<char, 4>::const_iterator end = std::cend(mathOperators);

	if (std::find(std::begin(mathOperators), end, character) == end)
	{
		return false;
	}

	if (source->peek() == '/')
	{
		return false;
	}

	std::map<char, TokenType>::const_iterator operatorType = MathOperatorsToTokenType.find(character);

	curToken.type = std::get<1>(*operatorType);

	return true;
}

const bool Lexer::tryToMakeConditionOperator(const char& character)
{
	if (character == '&')
	{
		if (source->peek() != '&')
			return false;

		source->getCharacter();
		curToken.type = TokenType::And;
		return true;
	}
	else if (character == '|')
	{
		if (source->peek() != '|')
			return false;

		source->getCharacter();
		curToken.type = TokenType::Or;
		return true;
	}

	return false;
}

const bool Lexer::tryToMakeRelationOperator(const char& character)
{
	if (character == '<')
	{
		if (source->peek() == '=')
		{
			source->getCharacter();
			curToken.type = TokenType::LessOrEqual;
			return true;
		}

		curToken.type = TokenType::Less;
		return true;
	}
	else if (character == '>')
	{
		if (source->peek() == '=')
		{
			source->getCharacter();
			curToken.type = TokenType::GreaterOrEqual;
			return true;
		}

		curToken.type = TokenType::Greater;
		return true;
	}
	else if (character == '=')
	{
		if (source->peek() == '=')
		{
			source->getCharacter();
			curToken.type = TokenType::Equal;
			return true;
		}

		curToken.type = TokenType::AssignOperator;
		return true;
	}
	else if (character == '!')
	{
		if (source->peek() == '=')
		{
			source->getCharacter();
			curToken.type = TokenType::NotEqual;
			return true;
		}

		curToken.type = TokenType::NotOperator;
		return true;
	}

	return false;
}

const bool Lexer::tryToMakeBracket(const char& character)
{
	if (character == '{')
	{
		curToken.type = TokenType::CurlyBracketOpen;
		return true;
	}
	else if (character == '}')
	{
		curToken.type = TokenType::CurlyBracketClose;
		return true;
	}
	else if (character == '(')
	{
		curToken.type = TokenType::RoundBracketOpen;
		return true;
	}
	else if (character == ')')
	{
		curToken.type = TokenType::RoundBracketClose;
		return true;
	}

	return false;
}

const bool Lexer::tryToMakeSymbols(const char& character)
{
	if (character == '.')
	{
		curToken.type = TokenType::Dot;
		return true;
	}
	else if (character == ';')
	{
		curToken.type = TokenType::Semicolon;
		return true;
	}
	else if (character == ',')
	{
		curToken.type = TokenType::Comma;
		return true;
	}

	return false;
}

const bool Lexer::isComment(const char& character)
{
	if (character != '/' || source->peek() != '/')
		return false;

	source->skipLine();
	return true;
}

