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

	if (consumeNextTokenIfType({ TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean }))
	{
		if (checkNextTokenType(TokenType::Function))
		{
			return parseDefFuncStatement(curToken.type);
		}
		else
		{
			std::shared_ptr<Node> declareVar = parseDeclareVarStatement(curToken.type);

			if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::Semicolon, LogType::MissingSemicolon, curToken))
			{
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

		if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::Semicolon, LogType::MissingSemicolon, curToken))
		{
			return nullptr;
		}

		return assignOrCallFuncStatement;
	}

	return nullptr;
}


std::shared_ptr<InstructionsBlock> Parser::parseInstructionsBlock()
{
	std::shared_ptr<InstructionsBlock> instructionsBlock = std::make_shared<InstructionsBlock>();

	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::CurlyBracketOpen, LogType::MissingCurlyBracketOpen))
	{
		return nullptr;
	}

	while (!checkNextTokenType(TokenType::CurlyBracketClose) && !checkNextTokenType(TokenType::EndOfFile))
	{
		std::shared_ptr<Node> node = parseInstructions();

		if (node != nullptr && node->getNodeType() == NodeType::DefFuncStatement)
		{
			logger->newLog(LogType::CantDefFuncInBlock, peekToken());
		}
		else
		{
			instructionsBlock->addInstruction(node);
		}
	}
	
	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::CurlyBracketClose, LogType::MissingCurlyBracketClose))
	{
		return nullptr;
	}

	return instructionsBlock;
}


std::shared_ptr<DefFuncStatement> Parser::parseDefFuncStatement(TokenType returnType)
{
	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::Function, LogType::MissingIdentifierOrFunctionKeyword))
	{
		return nullptr;
	}

	std::shared_ptr<DefFuncStatement> defFuncStatement = std::make_shared<DefFuncStatement>();

	if (returnType != TokenType::UNKNOWN)
	{
		defFuncStatement->setReturnType(returnType);
	}
	
	std::string name = peekToken().getStringValue();
	defFuncStatement->setName(name);

	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::Identifier, LogType::MissingIdentifier))
	{
		return nullptr;
	}

	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::RoundBracketOpen, LogType::MissingRoundBracketOpen))
	{
		return nullptr;
	}
	
	if (!consumeNextTokenIfType(TokenType::RoundBracketClose))
	{
		defFuncStatement->addParameter(parseParameter());

		while (consumeNextTokenIfType(TokenType::Comma))
		{
			if (checkNextTokenType(TokenType::RoundBracketClose))
			{
				logger->newLog(LogType::MissingParameter, peekToken());
				return nullptr;
			}

			defFuncStatement->addParameter(parseParameter());
		}

		if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
		{
			return nullptr;
		}
	}

	defFuncStatement->setInstructionsBlock(parseInstructionsBlock());

	return defFuncStatement;
}


std::shared_ptr<Parameter> Parser::parseParameter()
{
	std::shared_ptr<Parameter> parameter = std::make_shared<Parameter>();

	parameter->setType(peekToken().type);

	if (!consumeNextTokenIfType({ TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean }))
	{
		logger->newLog(LogType::BadSyntaxParameter, peekToken());
		return nullptr;
	}
	
	if (!checkNextTokenType(TokenType::Identifier))
	{
		logger->newLog(LogType::MissingIdentifier, peekToken());
		return nullptr;
	}

	Token token = getNextToken();
	parameter->setName(token.getStringValue());

	return parameter;
}


std::shared_ptr<IfStatement> Parser::parseIfStatement()
{
	if (!consumeNextTokenIfType(TokenType::If))
		return nullptr;

	std::shared_ptr<IfStatement> ifStatement = std::make_shared<IfStatement>();

	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::RoundBracketOpen, LogType::MissingRoundBracketOpen))
	{
		return nullptr;
	}

	ifStatement->setCondition(parseCondition());

	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
	{
		return nullptr;
	}

	ifStatement->setTrueBlockNode(parseInstructionsBlock());

	if (consumeNextTokenIfType(TokenType::Else))
	{
		ifStatement->setElseBlockNode(parseInstructionsBlock());
	}

	return ifStatement;
}


std::shared_ptr<RepeatStatement> Parser::parseRepeatStatement()
{
	if (!consumeNextTokenIfType(TokenType::Repeat))
		return nullptr;

	std::shared_ptr<RepeatStatement> repeatStatement = std::make_shared<RepeatStatement>();

	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::RoundBracketOpen, LogType::MissingRoundBracketOpen))
	{
		return nullptr;
	}

	repeatStatement->setHowManyTime(parseExpression());

	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
	{
		return nullptr;
	}

	repeatStatement->setInstructionsBlock(parseInstructionsBlock());

	return repeatStatement;
}


std::shared_ptr<RepeatTimeStatement> Parser::parseRepeatTimeStatement()
{
	if (!consumeNextTokenIfType(TokenType::RepeatTime))
		return nullptr;

	std::shared_ptr<RepeatTimeStatement> repeatTimeStatement = std::make_shared<RepeatTimeStatement>();

	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::RoundBracketOpen, LogType::MissingRoundBracketOpen))
	{
		return nullptr;
	}

	repeatTimeStatement->setPeriod(parseExpression());

	if (consumeNextTokenIfType(TokenType::Comma))
	{
		std::shared_ptr<Expression> howManyTime = parseExpression();
		repeatTimeStatement->setHowManyTime(howManyTime);
	}

	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
	{
		return nullptr;
	}

	repeatTimeStatement->setInstructionsBlock(parseInstructionsBlock());

	return repeatTimeStatement;
}

std::shared_ptr<Node> Parser::parseAssignOrCallFuncStatement()
{
	Token idToken = peekToken();
	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::Identifier, LogType::MissingIdentifier))
	{
		return nullptr;
	}

	std::vector<std::string> idNames;
	idNames.push_back(idToken.getStringValue());

	while (consumeNextTokenIfType(TokenType::Dot))
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
	if (!consumeNextTokenIfType(TokenType::RoundBracketOpen))
		return nullptr;

	std::shared_ptr<CallFuncStatement> callFunc = std::make_shared<CallFuncStatement>();

	if (!consumeNextTokenIfType(TokenType::RoundBracketClose))
	{
		callFunc->addArgument(parseExpression());

		while (consumeNextTokenIfType(TokenType::Comma))
		{
			callFunc->addArgument(parseExpression());
		}

		if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
		{
			return nullptr;
		}
	}

	callFunc->addIdentifiers(idNames);

	return callFunc;
}


std::shared_ptr<AssignStatement> Parser::parseAssignStatement(std::vector<std::string> idNames)
{
	if (!consumeNextTokenIfType(TokenType::AssignOperator))
		return nullptr;

	std::shared_ptr<AssignStatement> assign = std::make_shared<AssignStatement>();

	assign->setExpression(parseExpression());
	assign->addIdentifiers(idNames);

	return assign;
}

std::shared_ptr<AssignClassStatement> Parser::parseAssignClassStatement()
{
	if (!consumeNextTokenIfType(TokenType::RoundBracketOpen))
	{
		return nullptr;
	}

	std::shared_ptr<AssignClassStatement> assignClass = std::make_shared<AssignClassStatement>();
	assignClass->addExpression(parseExpression());

	while (consumeNextTokenIfType(TokenType::Comma))
	{
		if (checkNextTokenType(TokenType::RoundBracketClose))
		{
			logger->newLog(LogType::MissingParameter, peekToken());
		}

		assignClass->addExpression(parseExpression());
	}

	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
	{
		return nullptr;
	}
	
	return assignClass;
}


std::shared_ptr<DeclareVarStatement> Parser::parseDeclareVarStatement(TokenType type)
{
	if (!checkIfTokenTypeIsOneOf(type, { TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean }))
		return nullptr;

	std::string identifier = peekToken().getStringValue();

	if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::Identifier, LogType::MissingIdentifierOrFunctionKeyword))
	{
		return nullptr;
	}

	std::shared_ptr<DeclareVarStatement> varStatement = std::make_shared<DeclareVarStatement>();
	varStatement->setIdentifier(identifier);
	varStatement->setType(type);

	if (checkIfTokenTypeEqual(type, TokenType::Integer))
	{
		if (checkNextTokenType(TokenType::AssignOperator))
		{
			varStatement->setAssignStatement(parseAssignStatement({ identifier }));
		}
		
		return varStatement;
	}

	if (checkIfTokenTypeIsOneOf (type, { TokenType::Turtle, TokenType::Point }))
	{
		if (checkNextTokenType(TokenType::RoundBracketOpen))
		{
			varStatement->setAssignClassStatement(parseAssignClassStatement());
		}

		return varStatement;
	}

	if (checkIfTokenTypeEqual(type, TokenType::ColorVar))
	{
		if (consumeNextTokenIfType(TokenType::AssignOperator))
		{
			if (checkNextTokenType(TokenType::ColorValue))
			{
				Token colorVal = getNextToken();
				varStatement->setColorVal(colorVal.getStringValue());

				return varStatement;
			}
		}
	}
	// todo color boolean

	return varStatement;
}


std::shared_ptr<Expression> Parser::parseExpression()
{
	std::shared_ptr<Expression> expression = std::make_shared<Expression>();
	expression->addExpressionTerm(parseExpressionTerm());

	TokenType addOperator = peekToken().type;

	while (consumeNextTokenIfType({ TokenType::Plus, TokenType::Minus }))
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

	TokenType multiplyOperator = peekToken().type;

	while (consumeNextTokenIfType({ TokenType::Divide, TokenType::Multiply }))
	{
		term->addNextExpressionFactor(parseExpressionFactor(), multiplyOperator);
		multiplyOperator = peekToken().type;
	}

	return term;
}

std::shared_ptr<ExpressionFactor> Parser::parseExpressionFactor()
{
	std::shared_ptr<ExpressionFactor> factor = std::make_shared<ExpressionFactor>();
	

	if (consumeNextTokenIfType(TokenType::Minus))
	{
		factor->setNegativeOp(true);
	}

	Token token = peekToken();

	if (consumeNextTokenIfType(TokenType::RoundBracketOpen))
	{
		// expression in bracket
		factor->setExpression(parseExpression());

		if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
		{
			return nullptr;	
		}
	}
	else if (consumeNextTokenIfType(TokenType::Identifier))
	{
		// id var or call function
		std::vector<std::string> names;
		names.push_back(token.getStringValue());

		while (consumeNextTokenIfType(TokenType::Dot))
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

		// call function
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
			factor->setVariable(names);
		}
	}
	else if (consumeNextTokenIfType(TokenType::Digit))
	{
		factor->setIntVal(token.getIntValue());
	}
	else
	{
		logger->newLog(LogType::BadExpression, token);
		return nullptr;
	}

	return factor;
}

std::shared_ptr<Condition> Parser::parseCondition()
{
	std::shared_ptr<Condition> condition = std::make_shared<Condition>();
	condition->addAndCondition(parseAndCondition());

	while(consumeNextTokenIfType(TokenType::Or))
	{
		condition->addAndCondition(parseAndCondition());
	}

	return condition;
}

std::shared_ptr<AndCondition> Parser::parseAndCondition()
{
	std::shared_ptr<AndCondition> andCondition = std::make_shared<AndCondition>();
	andCondition->addRelationCondition(parseRelationCondition());

	while (consumeNextTokenIfType(TokenType::And))
	{
		andCondition->addRelationCondition(parseRelationCondition());
	}

	return andCondition;
}

std::shared_ptr<RelationCondition> Parser::parseRelationCondition()
{
	std::shared_ptr<RelationCondition> relationCondition = std::make_shared<RelationCondition>();
	
	if (consumeNextTokenIfType(TokenType::NotOperator))
	{
		relationCondition->setNotOperator(true);
	}

	Token token = peekToken();

	if (consumeNextTokenIfType({ TokenType::True, TokenType::False }))
	{
		relationCondition->setBooleanWord(token.type == TokenType::True);

		return relationCondition;
	}

	if (consumeNextTokenIfType(TokenType::RoundBracketOpen))
	{
		relationCondition->setCondition(parseCondition());

		if (!consumeNextTokenIfType_Otherwise_AddLog(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
		{
			return nullptr;
		}

		return relationCondition;
	}

	relationCondition->setExpression(parseExpression());
	token = peekToken();

	if (consumeNextTokenIfType({ TokenType::Equal, TokenType::NotEqual, TokenType::Less, TokenType::Greater, TokenType::LessOrEqual, TokenType::GreaterOrEqual }))
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

const bool Parser::consumeNextTokenIfType(const TokenType& type)
{
	return consumeNextTokenIfType(std::vector<TokenType>({ type }));
}

const bool Parser::consumeNextTokenIfType(const std::vector<TokenType>& types)
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

const bool Parser::consumeNextTokenIfType_Otherwise_AddLog(const TokenType& type, const LogType& logType, const Token& token)
{
	if (consumeNextTokenIfType(type))
		return true;

	logger->newLog(logType, token);

	return false;
}

const bool Parser::consumeNextTokenIfType_Otherwise_AddLog(const TokenType& type, const LogType& logType)
{
	return consumeNextTokenIfType_Otherwise_AddLog(type, logType, peekToken());
}