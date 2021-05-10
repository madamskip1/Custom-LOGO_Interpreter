#pragma once
#include <optional>
#include <vector>
#include "Lexer.h"
#include "Token.h"
#include "TokenType.h"
#include "ProgramRootNode.h"
#include "IfStatement.h"
#include "RepeatStatement.h"
#include "RepeatTimeStatement.h"
#include "Expression.h"

class Parser
{
public:
	Parser() = delete;
	Parser(Lexer* lex);

	std::unique_ptr<ProgramRootNode> parse();
private:
	Lexer* lexer;
	std::optional<Token> nextToken;


	std::unique_ptr<ProgramRootNode> parseProgram();

	std::shared_ptr<InstructionsBlock> parseInstructionsBlock();

	std::shared_ptr<IfStatement> parseIfStatement();
	std::shared_ptr<RepeatStatement> parseRepeatStatement();
	std::shared_ptr<RepeatTimeStatement> parseRepeatTimeStatement();


	std::shared_ptr<Expression> parseExpression();
	std::shared_ptr<ExpressionTerm> parseExpressionTerm();
	std::shared_ptr<ExpressionFactor> parseExpressionFactor();

	Token peekToken();
	Token getNextToken();
	bool hasBufferedToken();

	bool checkNextTokenType(const std::vector<TokenType>& types);
	bool checkIfTokenTypeIsOneOf(const TokenType& type, const std::vector<TokenType>& types);
};

