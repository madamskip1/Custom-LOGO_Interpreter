#include "Parser.h"
#include "FunctionDefNode.h"

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

	Token curToken = getNextToken();

	while (curToken.type != TokenType::EndOfFile)
	{
		if (curToken.type == TokenType::If)
		{
			std::shared_ptr<IfStatement> ifStatement = parseIfStatement();
			rootNode.get()->addInstruction(ifStatement);
		}
		else if (curToken.type == TokenType::Repeat)
		{
			std::shared_ptr<RepeatStatement> repeatStatement = parseRepeatStatement();
			rootNode.get()->addInstruction(repeatStatement);
		}
		else if (curToken.type == TokenType::RepeatTime)
		{
			std::shared_ptr<RepeatTimeStatement> RepeatTimeStatement = parseRepeatTimeStatement();
			rootNode.get()->addInstruction(RepeatTimeStatement);
		}

		curToken = getNextToken();
	}

	return rootNode;
}




std::shared_ptr<InstructionsBlock> Parser::parseInstructionsBlock()
{
	std::shared_ptr<InstructionsBlock> instructionsBlock = std::make_shared<InstructionsBlock>();

	if (!checkNextTokenType({ TokenType::CurlyBracketOpen }))
		return nullptr;
	getNextToken();

	// parseInstructions;

	if (!checkNextTokenType({ TokenType::CurlyBracketClose }))
		return nullptr;
	getNextToken();

	return instructionsBlock;
}

std::shared_ptr<IfStatement> Parser::parseIfStatement()
{
	std::shared_ptr<IfStatement> ifStatement = std::make_shared<IfStatement>();

	if (!checkNextTokenType({ TokenType::RoundBracketOpen }))
		return nullptr;

	getNextToken();

	// parseCondition

	// ifStatement->setCondtion

	getNextToken();

	if (!checkNextTokenType({ TokenType::RoundBracketClose }))
		return nullptr;
	getNextToken();

	std::shared_ptr<InstructionsBlock> trueBlock = parseInstructionsBlock();
	ifStatement.get()->setTrueBlockNode(trueBlock);

	if (checkNextTokenType({ TokenType::Else }))
	{
		getNextToken();

		std::shared_ptr<InstructionsBlock> elseBlock = parseInstructionsBlock();
		ifStatement.get()->setElseBlockNode(elseBlock);
	}

	return ifStatement;
}

std::shared_ptr<RepeatStatement> Parser::parseRepeatStatement()
{
	std::shared_ptr<RepeatStatement> repeatStatement = std::make_shared<RepeatStatement>();

	if (!checkNextTokenType({ TokenType::RoundBracketOpen }))
		return nullptr;

	getNextToken();

	std::shared_ptr<Expression> expression = parseExpression();
	repeatStatement->setHowManyTime(expression);

	if (!checkNextTokenType({ TokenType::RoundBracketClose }))
		return nullptr;
	getNextToken();

	// parse Block or Instruction

	// repeatStatement->setBlock

	return repeatStatement;
}

std::shared_ptr<RepeatTimeStatement> Parser::parseRepeatTimeStatement()
{
	std::shared_ptr<RepeatTimeStatement> repeatTimeStatement = std::make_shared<RepeatTimeStatement>();


	if (!checkNextTokenType({ TokenType::RoundBracketOpen }))
		return nullptr;

	getNextToken();

	// parseExpression
	// Musi byæ ewauluowany na liczbê!!!!!
	// RepeatTimeStatement->setPeriod

	if (checkNextTokenType({ TokenType::Comma }))
	{
		getNextToken();

		// parseExpression
		// Musi byæ ewaluowana na liczbê
		// RepeatTimeStatement->setHowManyTime
	}
	getNextToken();
	if (!checkNextTokenType({ TokenType::RoundBracketClose }))
		return nullptr;

	// parse Block or Instruction

	// RepeatTimeStatement->setBlock

	return repeatTimeStatement;
}


std::shared_ptr<Expression> Parser::parseExpression()
{
	std::shared_ptr<Expression> expression = std::make_shared<Expression>();
	expression.get()->addExpressionTerm(parseExpressionTerm());

	Token nextToken = peekToken();
	TokenType addOperator;
	while (nextToken.type == TokenType::Plus || nextToken.type == TokenType::Minus)
	{
		addOperator = nextToken.type;
		getNextToken();

		expression.get()->addNextExpressionTerm(parseExpressionTerm(), addOperator);
		nextToken = peekToken();
	}

	return expression;
}

std::shared_ptr<ExpressionTerm> Parser::parseExpressionTerm()
{
	std::shared_ptr<ExpressionTerm> term = std::make_shared<ExpressionTerm>();
	term.get()->addExpressionFactor(parseExpressionFactor());

	Token nextToken = peekToken();
	TokenType addOperator;

	while (nextToken.type == TokenType::Divide || nextToken.type == TokenType::Multiply)
	{
		addOperator = nextToken.type;
		getNextToken();

		term.get()->addNextExpressionFactor(parseExpressionFactor(), addOperator);
		nextToken = peekToken();
	}

	return term;
}

std::shared_ptr<ExpressionFactor> Parser::parseExpressionFactor()
{
	std::shared_ptr<ExpressionFactor> factor = std::make_shared<ExpressionFactor>();
	Token token = peekToken();


	if (token.type == TokenType::Minus)
	{
		getNextToken();
		token = peekToken();
		factor.get()->setNegativeOp(true);
	}

	if (token.type == TokenType::RoundBracketOpen)
	{
		getNextToken();
		factor.get()->setExpression(parseExpression());
		if (peekToken().type != TokenType::RoundBracketClose)
		{
			// ERROR BRAKUJE ZAMKNIECIA NAWIASU
		}
		else
		{
			getNextToken();
		}
	}
	else if (token.type == TokenType::Identifier)
	{
		// TODO
		// sprawdzic czy var czy funCall
		// ustawic opdowiendie
	}
	else if (token.type == TokenType::Digit)
	{
		factor.get()->setIntVal(token.getIntValue());
		getNextToken();
	}
	else
	{
		// Znaczy, ¿e z³e expression
	}

	return factor;
}

Token Parser::peekToken()
{
	if (nextToken)
		return *nextToken;

	nextToken = lexer->getNextToken();

	return *nextToken;
}

Token Parser::getNextToken()
{
	Token temp;

	if (nextToken)
	{
		temp = *nextToken;
		nextToken = std::nullopt;
	}
	else
	{
		temp = lexer->getNextToken();
	}

	return temp;
}

bool Parser::hasBufferedToken()
{
	return nextToken.has_value();
}

bool Parser::checkNextTokenType(const std::vector<TokenType>& types)
{
	Token curToken = peekToken();
	TokenType curTokenType = curToken.type;

	return checkIfTokenTypeIsOneOf(curTokenType, types);
}

bool Parser::checkIfTokenTypeIsOneOf(const TokenType& type, const std::vector<TokenType>& types)
{
	return (std::find(types.cbegin(), types.cend(), type) != types.cend());
}
