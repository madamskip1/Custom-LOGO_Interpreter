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
				rootNode.get()->addInstruction(defFunc);
			}
			else
			{
				// Deklaracja lub definicja zmiennych
				std::shared_ptr<DeclareVarStatement> varDec = parseDeclareVarStatement(curToken.type);
				rootNode.get()->addInstruction(varDec);
			}
		}
		else if (curToken.type == TokenType::Function)
		{
			std::shared_ptr<DefFuncStatement> defFunc = parseDefFuncStatement();
			rootNode.get()->addInstruction(defFunc);
		}
		else if (curToken.type == TokenType::If)
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
		else if (curToken.type == TokenType::Identifier)
		{
			std::shared_ptr<Node> assignOrCallFuncStatement = parseAssignOrCallFuncStatement();
			rootNode.get()->addInstruction(assignOrCallFuncStatement);

			if (peekToken().type == TokenType::Semicolon)
			{
				getNextToken();
			}
			else
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

	if (!checkNextTokenType({ TokenType::CurlyBracketOpen }))
		return nullptr;
	getNextToken();

	// parseInstructions;

	if (!checkNextTokenType({ TokenType::CurlyBracketClose }))
		return nullptr;
	getNextToken();

	return instructionsBlock;
}


std::shared_ptr<DefFuncStatement> Parser::parseDefFuncStatement(TokenType returnType)
{
	if (peekToken().type != TokenType::Function)
		return nullptr;
	getNextToken();

	std::shared_ptr<DefFuncStatement> defFuncStatement = std::make_shared<DefFuncStatement>();

	if (returnType != TokenType::UNKNOWN)
	{
		defFuncStatement.get()->setReturnType(returnType);
	}

	Token id = peekToken();

	if (id.type != TokenType::Identifier)
		return nullptr;

	getNextToken();
	std::string name = id.getStringValue();
	defFuncStatement.get()->setName(name);
	
	if (peekToken().type != TokenType::RoundBracketOpen)
	{
		// brak nawiasu otwierajacego
		// TO DO ERROR
	}
	getNextToken();

	if (peekToken().type != TokenType::RoundBracketClose)
	{
		defFuncStatement.get()->addParameter(parseParameter());
		Token nextToken = peekToken();

		while (nextToken.type == TokenType::Comma)
		{
			getNextToken();
			defFuncStatement.get()->addParameter(parseParameter());
			nextToken = peekToken();
		}

		if (nextToken.type == TokenType::RoundBracketClose)
		{
			getNextToken();
		}
		else
		{
			// JAKIS ERROR TO DO
			// Nie ma przecinka, a nastêpny token to nie zamykaj¹cy nawias
		}
	}
	else
	{
		getNextToken();
	}

	std::shared_ptr<InstructionsBlock> block = parseInstructionsBlock();
	defFuncStatement.get()->setInstructionsBlock(block);

	return defFuncStatement;
}

std::shared_ptr<Parameter> Parser::parseParameter()
{
	std::shared_ptr<Parameter> parameter = std::make_shared<Parameter>();

	Token tokenWithVarType = peekToken();

	if (!checkIfTokenTypeIsOneOf(tokenWithVarType.type, { TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean }))
	{
		// Parameter nie zaczyna siê od TYPU
		// TODO ERROR
		return nullptr;
	}
	getNextToken();
	parameter.get()->setType(tokenWithVarType.type);

	if (!checkNextTokenType({ TokenType::Identifier }))
	{
		// Brakuje nazwy parametru
		// TODO ERROR
		return nullptr;
	}

	Token id = getNextToken();
	std::string name = id.getStringValue();

	parameter.get()->setName(name);

	return parameter;
}



std::shared_ptr<IfStatement> Parser::parseIfStatement()
{
	if (peekToken().type != TokenType::If)
		return nullptr;
	getNextToken();

	std::shared_ptr<IfStatement> ifStatement = std::make_shared<IfStatement>();

	if (!checkNextTokenType({ TokenType::RoundBracketOpen }))
		return nullptr;

	getNextToken();

	ifStatement->setCondition(parseCondition());

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
	if (peekToken().type != TokenType::Repeat)
		return nullptr;
	getNextToken();

	std::shared_ptr<RepeatStatement> repeatStatement = std::make_shared<RepeatStatement>();

	if (!checkNextTokenType({ TokenType::RoundBracketOpen }))
		return nullptr;

	getNextToken();

	std::shared_ptr<Expression> expression = parseExpression();
	repeatStatement->setHowManyTime(expression);

	if (!checkNextTokenType({ TokenType::RoundBracketClose }))
		return nullptr;
	getNextToken();

	std::shared_ptr<InstructionsBlock> block = parseInstructionsBlock();
	repeatStatement.get()->setInstructionsBlock(block);

	return repeatStatement;
}

std::shared_ptr<RepeatTimeStatement> Parser::parseRepeatTimeStatement()
{
	if (peekToken().type != TokenType::RepeatTime)
		return nullptr;
	getNextToken();

	std::shared_ptr<RepeatTimeStatement> repeatTimeStatement = std::make_shared<RepeatTimeStatement>();


	if (!checkNextTokenType({ TokenType::RoundBracketOpen }))
		return nullptr;

	getNextToken();

	std::shared_ptr<Expression> period = parseExpression();
	repeatTimeStatement->setPeriod(period);

	if (checkNextTokenType({ TokenType::Comma }))
	{
		getNextToken();

		std::shared_ptr<Expression> howManyTime = parseExpression();
		repeatTimeStatement->setHowManyTime(howManyTime);
	}


	if (!checkNextTokenType({ TokenType::RoundBracketClose }))
		return nullptr;
	getNextToken();

	std::shared_ptr<InstructionsBlock> block = parseInstructionsBlock();
	repeatTimeStatement.get()->setInstructionsBlock(block);

	return repeatTimeStatement;
}

std::shared_ptr<Node> Parser::parseAssignOrCallFuncStatement()
{
	Token idToken = getNextToken();
	if (idToken.type != TokenType::Identifier)
	{
		// Nie zaczyna siê od identyfikatora
		return nullptr;
	}

	std::vector<std::string> idNames;
	idNames.push_back(idToken.getStringValue());

	while (peekToken().type == TokenType::Dot)
	{
		getNextToken();
		if (peekToken().type == TokenType::Identifier)
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

	if (peekToken().type == TokenType::RoundBracketOpen)
	{
		return parseCallFunc(idNames);
	}
	
	if (peekToken().type == TokenType::AssignOperator)
	{
		return parseAssignStatement(idNames);
	}


	
	//return std::shared_ptr<Node>();
	return nullptr;
}

std::shared_ptr<CallFuncStatement> Parser::parseCallFunc(std::vector<std::string> idNames)
{
	if (peekToken().type != TokenType::RoundBracketOpen)
		return nullptr;
	getNextToken();

	std::shared_ptr<CallFuncStatement> callFunc = std::make_shared<CallFuncStatement>();

	if (peekToken().type != TokenType::RoundBracketClose)
	{
		callFunc.get()->addArgument(parseExpression());

		Token nextToken = peekToken();
		while (nextToken.type == TokenType::Comma)
		{
			getNextToken();
			callFunc.get()->addArgument(parseExpression());
			nextToken = peekToken();
		}

		if (nextToken.type == TokenType::RoundBracketClose)
		{
			getNextToken();
		}
		else
		{
			// brakuje zamkniecia nawiasu
			// to do error
			return nullptr;
		}
	}
	else
	{
		getNextToken();
	}

	for (auto const& name : idNames)
		callFunc.get()->addIdentifier(name);

	return callFunc;
}

std::shared_ptr<AssignStatement> Parser::parseAssignStatement(std::vector<std::string> idNames)
{
	if (peekToken().type != TokenType::AssignOperator)
		return nullptr;
	getNextToken();

	std::shared_ptr<AssignStatement> assign = std::make_shared<AssignStatement>();

	assign.get()->setExpression(parseExpression());

	for (auto const& name : idNames)
		assign.get()->addIdentifier(name);

	return assign;
}

std::shared_ptr<DeclareVarStatement> Parser::parseDeclareVarStatement(TokenType type)
{
	if (!checkIfTokenTypeIsOneOf(type, { TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean }))
		return nullptr;

	Token id = peekToken();
	if (id.type != TokenType::Identifier)
		return nullptr;
	getNextToken();

	std::shared_ptr<DeclareVarStatement> varStatement = std::make_shared<DeclareVarStatement>();
	varStatement.get()->setIdentifier(id.getStringValue());
	varStatement.get()->setType(type);

	if (type != TokenType::Point && type != TokenType::Turtle)
	{
		if (peekToken().type == TokenType::AssignOperator)
		{
			varStatement.get()->setAssignStatement(parseAssignStatement({ id.getStringValue() }));
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
		std::vector<std::string> names;
		names.push_back(token.getStringValue());
		getNextToken();

		while (peekToken().type == TokenType::Comma)
		{
			getNextToken();
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

		if (peekToken().type == TokenType::RoundBracketOpen)
		{
			std::shared_ptr<CallFuncStatement> callFunc = parseCallFunc(names);
			if (callFunc == nullptr)
			{
				// Jakiœ error
				return nullptr;
			}

			factor.get()->setCallFunc(callFunc);
		}
		else
		{
			// TO IDENTYFIKATOR
			// TO DO
		}
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

std::shared_ptr<Condition> Parser::parseCondition()
{
	std::shared_ptr<Condition> condition = std::make_shared<Condition>();
	condition.get()->addAndCondition(parseAndCondition());

	while (peekToken().type == TokenType::Or)
	{
		getNextToken();
		condition.get()->addAndCondition(parseAndCondition());
	}

	return condition;
}

std::shared_ptr<AndCondition> Parser::parseAndCondition()
{
	std::shared_ptr<AndCondition> andCondition = std::make_shared<AndCondition>();
	andCondition.get()->addRelationCondition(parseRelationCondition());

	while (peekToken().type == TokenType::And)
	{
		getNextToken();
		andCondition.get()->addRelationCondition(parseRelationCondition());
	}

	return andCondition;
}

std::shared_ptr<RelationCondition> Parser::parseRelationCondition()
{
	std::shared_ptr<RelationCondition> relationCondition = std::make_shared<RelationCondition>();
	
	if (peekToken().type == TokenType::NotOperator)
	{
		getNextToken();
		relationCondition.get()->setNotOperator(true);
	}

	Token token = peekToken();

	if (token.type == TokenType::True)
	{
		relationCondition.get()->setBooleanWord(true);
		getNextToken();
	}
	else if (token.type == TokenType::False)
	{
		relationCondition.get()->setBooleanWord(false);
		getNextToken();
	}
	else if (token.type == TokenType::RoundBracketOpen)
	{
		getNextToken();
		relationCondition.get()->setCondition(parseCondition());

		if (peekToken().type != TokenType::RoundBracketClose)
		{
			// brak zamkniecia nawiasu
			// TO DO ERROR
			return nullptr;
		}
		getNextToken();
	}
	else
	{
		relationCondition.get()->setExpression(parseExpression());
		token = peekToken();
		if (checkIfTokenTypeIsOneOf(token.type, { TokenType::Equal, TokenType::NotEqual, TokenType::Less, TokenType::Greater, TokenType::LessOrEqual, TokenType::GreaterOrEqual }))
		{
			relationCondition.get()->setRelationOperator(token.type);
			getNextToken();
			relationCondition.get()->setExpression(parseExpression(), 2);
		}
	}

	return relationCondition;
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
