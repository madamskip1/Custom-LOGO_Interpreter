#include "Lexer.h"
#include <iostream>

Lexer::Lexer(SourceReader* reader) : source(reader)
{
}

Token Lexer::nextToken()
{
	token.type = TokenType::INVALID;
	char character;
	character = source->getCharacter();

	while (std::isspace(character))
		character = source->getCharacter();

	token.line = source->getLineNumber();
	token.firstCharPos = source->getCharNumber();

	if (source->isEof())
	{
		token.type = TokenType::EndOfFile;
		return token;
	}

	tryToMakeDigit(character);
	tryToMakeString(character);
	tryToMakeIDorKeywordOrDatatypes(character);
	tryToMakeMathOperator(character);
	tryToMakeConditionOperator(character);
	tryToRelationOperator(character);
	tryToMakeBracket(character);
	tryToMakeSymbols(character);
	tryToMakeComment(character);

	return token;
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

	token.value = val;
	token.type = TokenType::Digit;
}

const void Lexer::tryToMakeString(const char& character)
{
	if (character != '"') // string zaczyna siê cudzys³owiem
		return;

	char nextChar = source->peek();
	std::string string = "";

	while (nextChar != '"' && !source->isEof() && std::isprint(nextChar))
	{
		string += source->getCharacter();
		nextChar = source->peek();
	}

	if (nextChar != '"')
	{
		token.type = TokenType::INVALID;
		return;
	}

	source->getCharacter();
	token.type = TokenType::StringVal;
	token.value = string;
}

const void Lexer::tryToMakeIDorKeywordOrDatatypes(const char character)
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
		//token.type = KeywordToTokenType.at(name);
		token.type = std::get<1>(*keywordIterator);
	}
	else
	{
		token.type = TokenType::Identifier;
		token.value = name;
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
		// to komentarz
		return;
	}

	std::map<char, TokenType>::const_iterator operatorType = MathOperatorsToTokenType.find(character);
	token.type = std::get<1>(*operatorType);
}

const void Lexer::tryToMakeConditionOperator(const char& character)
{
	if (character == '&')
	{
		if (source->peek() != '&')
			return;

		source->getCharacter();
		token.type = TokenType::And;
	}
	else if (character == '|')
	{
		if (source->peek() != '|')
			return;

		source->getCharacter();
		token.type = TokenType::Or;
	}
}

const void Lexer::tryToRelationOperator(const char& character)
{
	if (character == '<')
	{
		if (source->peek() == '=')
		{
			source->getCharacter();
			token.type = TokenType::LessOrEqual;
			return;
		}

		token.type = TokenType::Less;
	}
	else if (character == '>')
	{
		if (source->peek() == '=')
		{
			source->getCharacter();
			token.type = TokenType::GreaterOrEqual;
			return;
		}

		token.type = TokenType::Greater;

	}
	else if (character == '=')
	{
		if (source->peek() == '=')
		{
			source->getCharacter();
			token.type = TokenType::Equal;
			return;
		}

		token.type = TokenType::AssignOperator;
	}
	else if (character == '!')
	{
		if (source->peek() == '=')
		{
			source->getCharacter();
			token.type = TokenType::NotEqual;
			return;
		}

		token.type = TokenType::NotOperator;
	}

}

const void Lexer::tryToMakeBracket(const char& character)
{
	if (character == '{')
	{
		token.type = TokenType::CurlyBracketOpen;
	} 
	else if (character == '}')
	{
		token.type = TokenType::CurlyBracketClose;
	}
	else if (character == '(')
	{
		token.type = TokenType::RoundBracketOpen;
	}
	else if (character == ')')
	{
		token.type = TokenType::RoundBracketClose;
	}

}

const void Lexer::tryToMakeSymbols(const char& character)
{
	if (character == '.')
	{
		token.type = TokenType::Dot;
	}
	else if (character == ';')
	{
		token.type = TokenType::Semicolon;
	}
	else if (character == ',')
	{
		token.type = TokenType::Comma;
	}
}

const void Lexer::tryToMakeComment(const char& character)
{
	if (character != '/' || source->peek() != '/')
		return;

	token.type = TokenType::Comment;
	source->skipLine();
}
