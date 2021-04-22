#include "Lexer.h"
#include <iostream>

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

	curToken.line = source->getLineNumber();
	curToken.firstCharPos = source->getCharNumber();

	if (source->isEof())
	{
		curToken.type = TokenType::EndOfFile;
		return curToken;
	}

	tryToMakeDigit(character);
	tryToMakeString(character);
	tryToMakeIDorKeywordOrDatatypes(character);
	tryToMakeMathOperator(character);
	tryToMakeConditionOperator(character);
	tryToMakeRelationOperator(character);
	tryToMakeBracket(character);
	tryToMakeSymbols(character);
	tryToMakeComment(character);

	return curToken;
}

const void Lexer::tryToMakeDigit(const char& character)
{
	if (!std::isdigit(character))
		return;

	int val = character - '0';
	
	while (std::isdigit(source->peek()))
	{
		val = val * 10 + (source->getCharacter() - '0');
	}

	curToken.type = TokenType::Digit;
	curToken.value = val;
}

const void Lexer::tryToMakeString(const char& character)
{
	if (character != '"')
		return;

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
		return;
	}

	source->getCharacter();

	curToken.type = TokenType::StringVal;
	curToken.value = string;
}

const void Lexer::tryToMakeIDorKeywordOrDatatypes(const char& character)
{
	if (!std::isalpha(character))
		return;
	
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
}

const void Lexer::tryToMakeMathOperator(const char& character)
{
	std::array<char, 4>::const_iterator end = std::cend(mathOperators);

	if (std::find(std::begin(mathOperators), end, character) == end)
	{
		return;
	}

	if (source->peek() == '/')
	{
		return;
	}

	std::map<char, TokenType>::const_iterator operatorType = MathOperatorsToTokenType.find(character);

	curToken.type = std::get<1>(*operatorType);
}

const void Lexer::tryToMakeConditionOperator(const char& character)
{
	if (character == '&')
	{
		if (source->peek() != '&')
			return;

		source->getCharacter();
		curToken.type = TokenType::And;
	}
	else if (character == '|')
	{
		if (source->peek() != '|')
			return;

		source->getCharacter();
		curToken.type = TokenType::Or;
	}
}

const void Lexer::tryToMakeRelationOperator(const char& character)
{
	if (character == '<')
	{
		if (source->peek() == '=')
		{
			source->getCharacter();
			curToken.type = TokenType::LessOrEqual;
			return;
		}

		curToken.type = TokenType::Less;
	}
	else if (character == '>')
	{
		if (source->peek() == '=')
		{
			source->getCharacter();
			curToken.type = TokenType::GreaterOrEqual;
			return;
		}

		curToken.type = TokenType::Greater;

	}
	else if (character == '=')
	{
		if (source->peek() == '=')
		{
			source->getCharacter();
			curToken.type = TokenType::Equal;
			return;
		}

		curToken.type = TokenType::AssignOperator;
	}
	else if (character == '!')
	{
		if (source->peek() == '=')
		{
			source->getCharacter();
			curToken.type = TokenType::NotEqual;
			return;
		}

		curToken.type = TokenType::NotOperator;
	}

}

const void Lexer::tryToMakeBracket(const char& character)
{
	if (character == '{')
	{
		curToken.type = TokenType::CurlyBracketOpen;
	} 
	else if (character == '}')
	{
		curToken.type = TokenType::CurlyBracketClose;
	}
	else if (character == '(')
	{
		curToken.type = TokenType::RoundBracketOpen;
	}
	else if (character == ')')
	{
		curToken.type = TokenType::RoundBracketClose;
	}

}

const void Lexer::tryToMakeSymbols(const char& character)
{
	if (character == '.')
	{
		curToken.type = TokenType::Dot;
	}
	else if (character == ';')
	{
		curToken.type = TokenType::Semicolon;
	}
	else if (character == ',')
	{
		curToken.type = TokenType::Comma;
	}
}

const void Lexer::tryToMakeComment(const char& character)
{
	if (character != '/' || source->peek() != '/')
		return;

	curToken.type = TokenType::Comment;
	source->skipLine();
}
