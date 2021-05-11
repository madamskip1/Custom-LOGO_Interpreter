#include "Parser.h"
#include "CallFuncStatement.h"
#include <iostream>
#include "Logger.h"

Parser::Parser(Lexer* lex, Logger* logger) : lexer(lex), logger(logger)
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
	std::shared_ptr<Node> temp;

	while (!checkNextTokenType(TokenType::EndOfFile))
	{
		temp = parseInstructions();

		if (temp != nullptr)
		{
			rootNode->addInstruction(temp);
		}
		else
		{
			getNextToken();
		}
	}

	return rootNode;
}

std::shared_ptr<Node> Parser::parseInstructions()
{
	Token curToken = peekToken();

	if (checkIfTokenTypeIsOneOf(curToken.type, { TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean }))
	{
		getNextToken();

		if (checkNextTokenType(TokenType::Function))
		{
			// Definicja Funkcji
			return parseDefFuncStatement(curToken.type);
		}
		else
		{
			// Deklaracja lub definicja zmiennych
			std::shared_ptr<Node> declareVar = parseDeclareVarStatement(curToken.type);

			if (!consumeNextTokenIfIsType(TokenType::Semicolon))
			{
				logger->newLog(LogType::MissingSemicolon, curToken);
				return nullptr;
			}

			return declareVar;
		}
	}
	
	if (checkIfTokenTypeEqual(curToken, TokenType::Function))
	{
		return parseDefFuncStatement();
	}
	
	if (checkIfTokenTypeEqual(curToken, TokenType::If))
	{
		return parseIfStatement();
	}
	
	if (checkIfTokenTypeEqual(curToken, TokenType::Repeat))
	{
		return parseRepeatStatement();
	}
	
	if (checkIfTokenTypeEqual(curToken, TokenType::RepeatTime))
	{
		return parseRepeatTimeStatement();
	}
	
	if (checkIfTokenTypeEqual(curToken, TokenType::Identifier))
	{
		std::shared_ptr<Node> assignOrCallFuncStatement = parseAssignOrCallFuncStatement();

		if (!consumeNextTokenIfIsType(TokenType::Semicolon))
		{
			// Brakuje œrednika
			// TO DO ERROR
		}

		return assignOrCallFuncStatement;
	}

	return nullptr; // coœ posz³o Ÿle
}


std::shared_ptr<InstructionsBlock> Parser::parseInstructionsBlock()
{
	std::shared_ptr<InstructionsBlock> instructionsBlock = std::make_shared<InstructionsBlock>();

	if (!consumeNextTokenIfIsType(TokenType::CurlyBracketOpen))
		return nullptr;


	std::shared_ptr<Node> node;

	while (!checkNextTokenType(TokenType::CurlyBracketClose) && !checkNextTokenType(TokenType::EndOfFile))
	{
		node = parseInstructions();
		if (node != nullptr && node->getNodeType() == NodeType::DefFuncStatement)
		{
			logger->newLog(LogType::CantDefFuncInBlock, peekToken());
		}
		else
		{
			instructionsBlock->addInstruction(node);
		}
	}

	if (!consumeNextTokenIfIsType(TokenType::CurlyBracketClose))
	{
		logger->newLog(LogType::MissingCurlyBracketClose, peekToken());
		return nullptr;
	}


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
	{
		logger->newLog(LogType::MissingIdentifier, peekToken());
		return nullptr;
	}

	defFuncStatement->setName(name);
	
	if (!consumeNextTokenIfIsType(TokenType::RoundBracketOpen))
	{
		logger->newLog(LogType::MissingRoundBracketOpen, peekToken());
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
			logger->newLog(LogType::MissingParameter, peekToken());
			return nullptr;
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
		logger->newLog(LogType::BadSyntaxParameter, peekToken());
		return nullptr;
	}

	parameter->setType(tokenType);

	if (!checkNextTokenType(TokenType::Identifier))
	{
		logger->newLog(LogType::MissingIdentifier, peekToken());
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
	{
		logger->newLog(LogType::MissingRoundBracketOpen, peekToken());
		return nullptr;
	}

	ifStatement->setCondition(parseCondition());

	if (!consumeNextTokenIfIsType(TokenType::RoundBracketClose))
	{	
		logger->newLog(LogType::MissingRoundBracketClose, peekToken());
		return nullptr;
	}

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
	{
		logger->newLog(LogType::MissingRoundBracketOpen, peekToken());
		return nullptr;
	}

	std::shared_ptr<Expression> expression = parseExpression();
	repeatStatement->setHowManyTime(expression);


	if (!consumeNextTokenIfIsType(TokenType::RoundBracketClose))
	{
		logger->newLog(LogType::MissingRoundBracketClose, peekToken());
		return nullptr;
	}

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
	{
		logger->newLog(LogType::MissingRoundBracketOpen, peekToken());
		return nullptr;
	}

	std::shared_ptr<Expression> period = parseExpression();
	repeatTimeStatement->setPeriod(period);

	if (consumeNextTokenIfIsType(TokenType::Comma))
	{
		std::shared_ptr<Expression> howManyTime = parseExpression();
		repeatTimeStatement->setHowManyTime(howManyTime);
	}

	if (!consumeNextTokenIfIsType(TokenType::RoundBracketClose))
	{
		logger->newLog(LogType::MissingRoundBracketClose, peekToken());
		return nullptr;
	}

	std::shared_ptr<InstructionsBlock> block = parseInstructionsBlock();
	repeatTimeStatement->setInstructionsBlock(block);

	return repeatTimeStatement;
}

std::shared_ptr<Node> Parser::parseAssignOrCallFuncStatement()
{
	Token idToken = getNextToken();

	if (!checkIfTokenTypeEqual(idToken, TokenType::Identifier))
	{
		logger->newLog(LogType::MissingIdentifier, peekToken());
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
			logger->newLog(LogType::MissingIdentifier, peekToken());
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
			logger->newLog(LogType::MissingRoundBracketClose, peekToken());
			return nullptr;
		}
	}

	callFunc->addIdentifiers(idNames);

	return callFunc;
}

std::shared_ptr<AssignStatement> Parser::parseAssignStatement(std::vector<std::string> idNames)
{
	if (!consumeNextTokenIfIsType(TokenType::AssignOperator))
		return nullptr;

	std::shared_ptr<AssignStatement> assign = std::make_shared<AssignStatement>();

	assign->setExpression(parseExpression());
	assign->addIdentifiers(idNames);

	return assign;
}

std::shared_ptr<DeclareVarStatement> Parser::parseDeclareVarStatement(TokenType type)
{
	if (!checkIfTokenTypeIsOneOf(type, { TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean }))
		return nullptr;

	std::string identifier = peekToken().getStringValue();

	if (!consumeNextTokenIfIsType(TokenType::Identifier))
	{
		logger->newLog(LogType::MissingIdentifierOrFunctionKeyword, peekToken());
		return nullptr;
	}


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
			logger->newLog(LogType::MissingRoundBracketClose, peekToken());
			return nullptr;
		}
	}
	else if (consumeNextTokenIfIsType(TokenType::Identifier))
	{
		std::vector<std::string> names;
		names.push_back(token.getStringValue());

		while (consumeNextTokenIfIsType(TokenType::Dot))
		{
			if (peekToken().type == TokenType::Identifier)
			{
				token = getNextToken();
				names.push_back(token.getStringValue());
			}
			else
			{
				logger->newLog(LogType::MissingIdentifier, peekToken());
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
		logger->newLog(LogType::BadExpression, token);
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
	
	if (consumeNextTokenIfIsType(TokenType::NotOperator))
	{
		relationCondition->setNotOperator(true);
	}

	Token token = peekToken();

	if (consumeNextTokenIfIsType({ TokenType::True, TokenType::False }))
	{
		relationCondition->setBooleanWord(token.type == TokenType::True);

		return relationCondition;
	}

	if (consumeNextTokenIfIsType(TokenType::RoundBracketOpen))
	{
		relationCondition->setCondition(parseCondition());

		if (!checkIfTokenTypeEqual(peekToken(), TokenType::RoundBracketClose))
		{
			logger->newLog(LogType::MissingRoundBracketClose, peekToken());
			return nullptr;
		}
		getNextToken();

		return relationCondition;
	}

	relationCondition->setExpression(parseExpression());
	token = peekToken();

	if (consumeNextTokenIfIsType({ TokenType::Equal, TokenType::NotEqual, TokenType::Less, TokenType::Greater, TokenType::LessOrEqual, TokenType::GreaterOrEqual }))
	{
		relationCondition->setRelationOperator(token.type);
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
