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
#include "DefFuncStatement.h"
#include "Parameter.h"
#include "Condition.h"
#include "AssignStatement.h"
#include "DeclareVarStatement.h"
#include "Logger.h"
#include "AssignClassStatement.h"

class Parser
{
public:
	Parser() = delete;
	Parser(Lexer* lex, Logger* logger);

	std::unique_ptr<ProgramRootNode> parse();

private:
	Lexer* lexer;
	Logger* logger;
	std::optional<Token> nextToken;


	std::unique_ptr<ProgramRootNode> parseProgram();

	std::shared_ptr<Node> parseInstruction();

	std::shared_ptr<InstructionsBlock> parseInstructionsBlock();
	std::shared_ptr<Node> parseDeclareVarORDefFuncWithReturStatement();
	std::shared_ptr<DefFuncStatement> parseDefFuncStatement(TokenType returnType = TokenType::UNKNOWN);
	std::shared_ptr<Parameter> parseParameter();
	std::shared_ptr<IfStatement> parseIfStatement();
	std::shared_ptr<RepeatStatement> parseRepeatStatement();
	std::shared_ptr<RepeatTimeStatement> parseRepeatTimeStatement();

	std::shared_ptr<Node> parseAssignOrCallFuncStatement();
	std::shared_ptr<CallFuncStatement> parseCallFunc(std::vector<std::string> idNames);
	std::shared_ptr<AssignStatement> parseAssignStatement(std::vector<std::string> idNames);
	std::shared_ptr<AssignClassStatement> parseAssignClassStatement();
	std::shared_ptr<DeclareVarStatement> parseDeclareVarStatement(TokenType type);

	std::vector<std::string> parseIdentifiers();

	std::shared_ptr<Expression> parseExpression();
	std::shared_ptr<TermExpression> parseExpressionTerm();
	std::shared_ptr<FactorExpression> parseExpressionFactor();

	std::shared_ptr<Condition> parseCondition();
	std::shared_ptr<AndCondition> parseAndCondition();
	std::shared_ptr<RelationCondition> parseRelationCondition();

	Token peekToken();
	Token getNextToken();
	const bool hasBufferedToken() const;

	
	const bool checkNextTokenType(const TokenType& type);
	const bool checkNextTokenType(const std::vector<TokenType>& types);
	const bool consumeNextTokenIfType(const TokenType& type);
	const bool consumeNextTokenIfType(const std::vector<TokenType>& types);
	const bool checkIfTokenTypeIsOneOf(const TokenType& type, const std::vector<TokenType>& types) const;
	const bool checkIfTokenTypeEqual(const TokenType& tokenType, const TokenType& type) const;
	const bool checkIfTokenTypeEqual(const Token& token, const TokenType& type) const;

	const bool consumeNextTokenIfType_Otherwise_AddLog(const TokenType& type, const LogType& logType, const Token& token);
	const bool consumeNextTokenIfType_Otherwise_AddLog(const TokenType& type, const LogType& logType);
};

