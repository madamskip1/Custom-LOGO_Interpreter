#include "Parser.h"
#include "CallFuncStatement.h"
#include <iostream>

Parser::Parser(Lexer* lex) : lexer(lex)
{
	nextToken = std::nullopt;
}

std::unique_ptr<ProgramRootNode> Parser::parse()
{
	return parseProgram();
}

std::unique_ptr<ProgramRootNode> Parser::parseProgram()
{
	std::unique_ptr<ProgramRootNode> rootNode = std::make_unique<ProgramRootNode>();

	Token curToken = peekToken();

	while (curToken.type != TokenType::EndOfFile)
	{
		if (checkIfTokenTypeIsOneOf(curToken.type, { TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean }))
		{
			getNextToken();
			if (peekToken().type == TokenType::Function)
			{
				// Definicja Funkcji
				std::shared_ptr<DefFuncStatement> defFunc = parseDefFuncStatement(curToken.type);
				rootNode->addInstruction(defFunc);
			}
			else
			{
				// Deklaracja lub definicja zmiennych
				std::shared_ptr<DeclareVarStatement> varDec = parseDeclareVarStatement(curToken.type);
				rootNode->addInstruction(varDec);
			}
		}
		else if (curToken.type == TokenType::Function)
		{
			std::shared_ptr<DefFuncStatement> defFunc = parseDefFuncStatement();
			rootNode->addInstruction(defFunc);
		}
		else if (curToken.type == TokenType::If)
		{
			std::shared_ptr<IfStatement> ifStatement = parseIfStatement();
			rootNode->addInstruction(ifStatement);
		}
		else if (curToken.type == TokenType::Repeat)
		{
			std::shared_ptr<RepeatStatement> repeatStatement = parseRepeatStatement();
			rootNode->addInstruction(repeatStatement);
		}
		else if (curToken.type == TokenType::RepeatTime)
		{
			std::shared_ptr<RepeatTimeStatement> RepeatTimeStatement = parseRepeatTimeStatement();
			rootNode->addInstruction(RepeatTimeStatement);
		}
		else if (curToken.type == TokenType::Identifier)
		{
			std::shared_ptr<Node> assignOrCallFuncStatement = parseAssignOrCallFuncStatement();
			rootNode->addInstruction(assignOrCallFuncStatement);

			if (!consumeNextTokenIfIsType(TokenType::Semicolon))
			{
				// Brakuje œrednika
				// TO DO ERROR
			}
		}
		else
		{
			// brak dopasowania
			// TODO ERROR
			getNextToken();
		}

		curToken = peekToken();
	}

	return rootNode;
}




std::shared_ptr<InstructionsBlock> Parser::parseInstructionsBlock()
{
	std::shared_ptr<InstructionsBlock> instructionsBlock = std::make_shared<InstructionsBlock>();

	if (!consumeNextTokenIfIsType(TokenType::CurlyBracketOpen))
		return nullptr;

	// parseInstructions;

	if (!consumeNextTokenIfIsType(TokenType::CurlyBracketClose))
		return nullptr;

	return instructionsBlock;
}


std::shared_ptr<DefFuncStatement> Parser::parseDefFuncStatement(TokenType returnType)
{
	if (!consumeNextTokenIfIsType(TokenType::Function))
		return nullptr;

	std::shared_ptr<DefFuncStatement> defFuncStatement = std::make_shared<DefFuncStatement>();

	if (returnType != TokenType::UNKNOWN)
	{
		defFuncStatement->setReturnType(returnType);
	}
	
	std::string name = peekToken().getStringValue();

	if (!consumeNextTokenIfIsType(TokenType::Identifier))
		return nullptr;

	defFuncStatement->setName(name);
	
	if (!consumeNextTokenIfIsType(TokenType::RoundBracketOpen))
	{
		// brak nawiasu otwierajacego
		// TO DO ERROR
		return nullptr;
	}
	
	if (!consumeNextTokenIfIsType(TokenType::RoundBracketClose))
	{
		defFuncStatement->addParameter(parseParameter());
		Token nextToken = peekToken();

		while (consumeNextTokenIfIsType(TokenType::Comma))
		{
			defFuncStatement->addParameter(parseParameter());
			nextToken = peekToken();
		}

		if(!consumeNextTokenIfIsType(TokenType::RoundBracketClose))
		{
			// JAKIS ERROR TO DO
			// Nie ma przecinka, a nastêpny token to nie zamykaj¹cy nawias
		}
	}

	std::shared_ptr<InstructionsBlock> block = parseInstructionsBlock();
	defFuncStatement->setInstructionsBlock(block);

	return defFuncStatement;
}


std::shared_ptr<Parameter> Parser::parseParameter()
{
	std::shared_ptr<Parameter> parameter = std::make_shared<Parameter>();

	TokenType tokenType = peekToken().type;

	if (!consumeNextTokenIfIsType({ TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean }))
	{
		// Parameter nie zaczyna siê od TYPU
		// TODO ERROR
		return nullptr;
	}

	parameter->setType(tokenType);

	if (!checkNextTokenType(TokenType::Identifier))
	{
		// Brakuje nazwy parametru
		// TODO ERROR
		return nullptr;
	}

	Token id = getNextToken();
	std::string name = id.getStringValue();

	parameter->setName(name);

	return parameter;
}



std::shared_ptr<IfStatement> Parser::parseIfStatement()
{
	if (!consumeNextTokenIfIsType(TokenType::If))
		return nullptr;

	std::shared_ptr<IfStatement> ifStatement = std::make_shared<IfStatement>();

	if (!consumeNextTokenIfIsType(TokenType::RoundBracketOpen))
		return nullptr;

	ifStatement->setCondition(parseCondition());

	if (!consumeNextTokenIfIsType(TokenType::RoundBracketClose))
		return nullptr;

	std::shared_ptr<InstructionsBlock> trueBlock = parseInstructionsBlock();
	ifStatement->setTrueBlockNode(trueBlock);

	if (consumeNextTokenIfIsType(TokenType::Else))
	{
		std::shared_ptr<InstructionsBlock> elseBlock = parseInstructionsBlock();
		ifStatement->setElseBlockNode(elseBlock);
	}

	return ifStatement;
}

std::shared_ptr<RepeatStatement> Parser::parseRepeatStatement()
{
	if (!consumeNextTokenIfIsType(TokenType::Repeat))
		return nullptr;

	std::shared_ptr<RepeatStatement> repeatStatement = std::make_shared<RepeatStatement>();

	if (!consumeNextTokenIfIsType(TokenType::RoundBracketOpen))
		return nullptr;

	std::shared_ptr<Expression> expression = parseExpression();
	repeatStatement->setHowManyTime(expression);


	if (!consumeNextTokenIfIsType(TokenType::RoundBracketClose))
		return nullptr;

	std::shared_ptr<InstructionsBlock> block = parseInstructionsBlock();
	repeatStatement->setInstructionsBlock(block);

	return repeatStatement;
}

std::shared_ptr<RepeatTimeStatement> Parser::parseRepeatTimeStatement()
{
	if (!consumeNextTokenIfIsType(TokenType::RepeatTime))
		return nullptr;

	std::shared_ptr<RepeatTimeStatement> repeatTimeStatement = std::make_shared<RepeatTimeStatement>();

	if (!consumeNextTokenIfIsType(TokenType::RoundBracketOpen))
		return nullptr;

	std::shared_ptr<Expression> period = parseExpression();
	repeatTimeStatement->setPeriod(period);

	if (consumeNextTokenIfIsType(TokenType::Comma))
	{
		std::shared_ptr<Expression> howManyTime = parseExpression();
		repeatTimeStatement->setHowManyTime(howManyTime);
	}

	if (!consumeNextTokenIfIsType(TokenType::RoundBracketClose))
		return nullptr;

	std::shared_ptr<InstructionsBlock> block = parseInstructionsBlock();
	repeatTimeStatement->setInstructionsBlock(block);

	return repeatTimeStatement;
}

std::shared_ptr<Node> Parser::parseAssignOrCallFuncStatement()
{
	Token idToken = getNextToken();

	if (!checkIfTokenTypeEqual(idToken, TokenType::Identifier))
	{
		// Nie zaczyna siê od identyfikatora
		return nullptr;
	}

	std::vector<std::string> idNames;
	idNames.push_back(idToken.getStringValue());

	while (consumeNextTokenIfIsType(TokenType::Dot))
	{
		if (checkNextTokenType(TokenType::Identifier))
		{
			idToken = getNextToken();
			idNames.push_back(idToken.getStringValue());
		}
		else
		{
			// Powinien byæ identyfikator
			// TODO ERROR
			return nullptr;
		}
	}

	if (checkNextTokenType(TokenType::RoundBracketOpen))
	{
		return parseCallFunc(idNames);
	}
	
	if (checkNextTokenType(TokenType::AssignOperator))
	{
		return parseAssignStatement(idNames);
	}

	return nullptr;
}

std::shared_ptr<CallFuncStatement> Parser::parseCallFunc(std::vector<std::string> idNames)
{
	if (!consumeNextTokenIfIsType(TokenType::RoundBracketOpen))
		return nullptr;

	std::shared_ptr<CallFuncStatement> callFunc = std::make_shared<CallFuncStatement>();

	if (!consumeNextTokenIfIsType(TokenType::RoundBracketClose))
	{
		callFunc->addArgument(parseExpression());

		Token nextToken = peekToken();

		while (consumeNextTokenIfIsType(TokenType::Comma))
		{
			callFunc->addArgument(parseExpression());
			nextToken = peekToken();
		}

		if (!consumeNextTokenIfIsType(TokenType::RoundBracketClose))
		{
			// brakuje zamkniecia nawiasu
			// to do error
			return nullptr;
		}
	}

	for (auto const& name : idNames)
		callFunc->addIdentifier(name);

	return callFunc;
}

std::shared_ptr<AssignStatement> Parser::parseAssignStatement(std::vector<std::string> idNames)
{
	if (!consumeNextTokenIfIsType(TokenType::AssignOperator))
		return nullptr;

	std::shared_ptr<AssignStatement> assign = std::make_shared<AssignStatement>();

	assign->setExpression(parseExpression());

	for (auto const& name : idNames)
		assign->addIdentifier(name);

	return assign;
}

std::shared_ptr<DeclareVarStatement> Parser::parseDeclareVarStatement(TokenType type)
{
	if (!checkIfTokenTypeIsOneOf(type, { TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean }))
		return nullptr;

	std::string identifier = peekToken().getStringValue();

	if (!consumeNextTokenIfIsType(TokenType::Identifier))
		return nullptr;


	std::shared_ptr<DeclareVarStatement> varStatement = std::make_shared<DeclareVarStatement>();
	varStatement->setIdentifier(identifier);
	varStatement->setType(type);

	if (!checkIfTokenTypeIsOneOf(type, { TokenType::Point, TokenType::Turtle }))
	{
		if (checkNextTokenType(TokenType::AssignOperator))
		{
			varStatement->setAssignStatement(parseAssignStatement({ identifier }));
		}
	}
	else
	{
		// TO DO
		// Point, Turtle troche inaczej mo¿e?, bo test(2, 2)
	}

	return varStatement;
}



std::shared_ptr<Expression> Parser::parseExpression()
{
	std::shared_ptr<Expression> expression = std::make_shared<Expression>();
	expression->addExpressionTerm(parseExpressionTerm());

	TokenType addOperator = peekToken().type;

	while (consumeNextTokenIfIsType({ TokenType::Plus, TokenType::Minus }))
	{
		expression->addNextExpressionTerm(parseExpressionTerm(), addOperator);
		addOperator = peekToken().type;
	}

	return expression;
}

std::shared_ptr<ExpressionTerm> Parser::parseExpressionTerm()
{
	std::shared_ptr<ExpressionTerm> term = std::make_shared<ExpressionTerm>();
	term->addExpressionFactor(parseExpressionFactor());

	TokenType addOperator = peekToken().type;

	while (consumeNextTokenIfIsType({ TokenType::Divide, TokenType::Multiply }))
	{
		term->addNextExpressionFactor(parseExpressionFactor(), addOperator);
		addOperator = peekToken().type;
	}

	return term;
}

std::shared_ptr<ExpressionFactor> Parser::parseExpressionFactor()
{
	std::shared_ptr<ExpressionFactor> factor = std::make_shared<ExpressionFactor>();
	

	if (consumeNextTokenIfIsType(TokenType::Minus))
	{
		factor->setNegativeOp(true);
	}

	Token token = peekToken();

	if (consumeNextTokenIfIsType(TokenType::RoundBracketOpen))
	{
		factor->setExpression(parseExpression());

		if (!consumeNextTokenIfIsType(TokenType::RoundBracketClose))
		{
			// ERROR BRAKUJE ZAMKNIECIA NAWIASU
			return nullptr;
		}
	}
	else if (consumeNextTokenIfIsType(TokenType::Identifier))
	{
		std::vector<std::string> names;
		names.push_back(token.getStringValue());

		while (consumeNextTokenIfIsType(TokenType::Comma))
		{
			if (peekToken().type == TokenType::Identifier)
			{
				token = getNextToken();
				names.push_back(token.getStringValue());
			}
			else
			{
				// Powinien byæ identyfikator
				// TO DOO ERROR
				return nullptr;
			}
		}

		if (checkNextTokenType(TokenType::RoundBracketOpen))
		{
			std::shared_ptr<CallFuncStatement> callFunc = parseCallFunc(names);

			if (callFunc == nullptr)
			{
				// Jakiœ error
				return nullptr;
			}

			factor->setCallFunc(callFunc);
		}
		else
		{
			// TO IDENTYFIKATOR
			// TO DO
		}
	}
	else if (consumeNextTokenIfIsType(TokenType::Digit))
	{
		factor->setIntVal(token.getIntValue());
	}
	else
	{
		// Znaczy, ¿e z³e expression
	}

	return factor;
}

std::shared_ptr<Condition> Parser::parseCondition()
{
	std::shared_ptr<Condition> condition = std::make_shared<Condition>();
	condition->addAndCondition(parseAndCondition());

	while(consumeNextTokenIfIsType(TokenType::Or))
	{
		condition->addAndCondition(parseAndCondition());
	}

	return condition;
}

std::shared_ptr<AndCondition> Parser::parseAndCondition()
{
	std::shared_ptr<AndCondition> andCondition = std::make_shared<AndCondition>();
	andCondition->addRelationCondition(parseRelationCondition());

	while (consumeNextTokenIfIsType(TokenType::And))
	{
		andCondition->addRelationCondition(parseRelationCondition());
	}

	return andCondition;
}

std::shared_ptr<RelationCondition> Parser::parseRelationCondition()
{
	std::shared_ptr<RelationCondition> relationCondition = std::make_shared<RelationCondition>();
	
	if (checkIfTokenTypeEqual(peekToken(), TokenType::NotOperator))
	{
		getNextToken();
		relationCondition->setNotOperator(true);
	}

	Token token = peekToken();

	if (checkIfTokenTypeIsOneOf(token.type, { TokenType::True, TokenType::False }))
	{
		relationCondition->setBooleanWord(token.type == TokenType::True);
		getNextToken();

		return relationCondition;
	}

	if (checkIfTokenTypeEqual(token.type, TokenType::RoundBracketOpen))
	{
		getNextToken();
		relationCondition->setCondition(parseCondition());

		if (!checkIfTokenTypeEqual(peekToken(), TokenType::RoundBracketClose))
		{
			// brak zamkniecia nawiasu
			// TO DO ERROR
			return nullptr;
		}
		getNextToken();

		return relationCondition;
	}

	relationCondition->setExpression(parseExpression());
	token = peekToken();

	if (checkIfTokenTypeIsOneOf(token.type, { TokenType::Equal, TokenType::NotEqual, TokenType::Less, TokenType::Greater, TokenType::LessOrEqual, TokenType::GreaterOrEqual }))
	{
		relationCondition->setRelationOperator(token.type);
		getNextToken();
		relationCondition->setExpression(parseExpression(), 2);
	}

	return relationCondition;
}

Token Parser::peekToken()
{
	if (hasBufferedToken())
		return *nextToken;

	nextToken = lexer->getNextToken();

	return *nextToken;
}

Token Parser::getNextToken()
{
	if (hasBufferedToken())
	{
		Token temp;
		temp = *nextToken;
		nextToken = std::nullopt;

		return temp;
	}

	return lexer->getNextToken();
}

const bool Parser::hasBufferedToken() const
{
	return nextToken.has_value();
}

const bool Parser::consumeNextTokenIfIsType(const TokenType& type)
{
	return consumeNextTokenIfIsType(std::vector<TokenType>({ type }));
}

const bool Parser::consumeNextTokenIfIsType(const std::vector<TokenType>& types)
{
	if (checkNextTokenType(types))
	{
		getNextToken();
		return true;
	}
	
	return false;
}

const bool Parser::checkNextTokenType(const TokenType& type)
{
	return checkNextTokenType(std::vector<TokenType>({ type }));
}

const bool Parser::checkNextTokenType(const std::vector<TokenType>& types)
{
	return checkIfTokenTypeIsOneOf(peekToken().type, types);
}

const bool Parser::checkIfTokenTypeIsOneOf(const TokenType& type, const std::vector<TokenType>& types) const
{
	return (std::find(types.cbegin(), types.cend(), type) != types.cend());
}

const bool Parser::checkIfTokenTypeEqual(const TokenType& tokenType, const TokenType& type) const
{
	return tokenType == type;
}

const bool Parser::checkIfTokenTypeEqual(const Token& token, const TokenType& type) const
{
	return checkIfTokenTypeEqual(token.type, type);
}
