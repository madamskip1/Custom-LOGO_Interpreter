#pragma once
#include <optional>
#include <memory>
#include <vector>
#include <string>
#include "Lexer.h"
#include "Logger.h"
#include "Token.h"

#include "ProgramRootNode.h"
#include "Node.h"
#include "InstructionsBlock.h"
#include "Expression.h"
#include "Condition.h"
#include "Parameter.h"
#include "VarDeclare.h"
#include "CallFuncStatement.h"
#include "AssignmentStatement.h"
#include "ClassAssignment.h"
#include "ReturnStatement.h"


class Parser
{
public:
	Parser() = delete;
	Parser(Lexer& lex, Logger& logger);

	std::unique_ptr<Node> parse();

private:
	Lexer& lexer;
	Logger& logger;
	std::optional<Token> token;
	
	std::unique_ptr<Node> parseProgram();

	std::unique_ptr<Node> parseInstruction();
	std::unique_ptr<InstructionsBlock> parseInstructionsBlock();
	std::unique_ptr<Node> parseIfStatement();
	std::unique_ptr<Node> parseRepeatStatement();
	std::unique_ptr<Node> parseRepeatTimeStatement();
	std::unique_ptr<Node> parseAssignOrCallFunctionStatement();
	std::unique_ptr<CallFuncStatement> parseCallFunctionStatement(std::vector<std::string> identifiers);

	std::unique_ptr<Node> parseVarDeclareORDefFuncWithReturStatement();
	std::unique_ptr<VarDeclare> parseVarDeclare(const TokenType& type);
	std::unique_ptr<Node> parseDefFuncStatement(const TokenType& returnType = TokenType::UNKNOWN);

	std::unique_ptr<AssignmentStatement> parseAssignment(std::vector<std::string> identifiers);
	std::unique_ptr<Assignable> parseAssignable();
	std::unique_ptr<ClassAssignment> parseClassAssignment();

	std::unique_ptr<Parameter> parseParameter();

	std::unique_ptr<Expression> parseExpression();
	std::unique_ptr<Expression> parseTermExpression();
	std::unique_ptr<Expression> parseFactorExpression();

	std::unique_ptr<Node> parseCondition();
	std::unique_ptr<Node> parseAndCondition();
	std::unique_ptr<Node> parseRelationCondition();

	std::unique_ptr<ReturnStatement> parseReturnStatement();

	std::vector<std::string> parseIdentifiers();

	Token getToken();
	Token getAndConsumeToken();
	const void consumeToken();

	const bool consumeTokenIfType(const TokenType& type);
	const bool consumeTokenIfType(const std::vector<TokenType>& types);

	const bool consumeTokenIfType_Otherwise_AddLog(const TokenType& tokenType, LogType logType);
	const bool consumeTokenIfType_Otherwise_AddError(const TokenType& tokenType, LogType logType);

	const bool checkCurTokenType(const TokenType& type);
	const bool checkCurTokenType(const std::vector<TokenType>& types);

	const bool checkTokenType(const TokenType& tokenType, const TokenType& type);
	const bool checkTokenType(const TokenType& tokenType, const std::vector<TokenType>& types);
};