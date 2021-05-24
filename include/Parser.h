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

	std::unique_ptr<AST::ProgramRootNode> parse();

private:
	Lexer& lexer;
	Logger& logger;
	std::optional<Token> token;
	
	std::unique_ptr<AST::ProgramRootNode> parseProgram();

	std::unique_ptr<AST::Node> parseInstruction();
	std::unique_ptr<AST::InstructionsBlock> parseInstructionsBlock();
	std::unique_ptr<AST::Node> parseIfStatement();
	std::unique_ptr<AST::Node> parseRepeatStatement();
	std::unique_ptr<AST::Node> parseRepeatTimeStatement();
	std::unique_ptr<AST::Node> parseAssignOrCallFunctionStatement();
	std::unique_ptr<AST::CallFuncStatement> parseCallFunctionStatement(std::vector<std::string> identifiers);

	std::unique_ptr<AST::Node> parseVarDeclareORDefFuncWithReturStatement();
	std::unique_ptr<AST::VarDeclare> parseVarDeclare(const TokenType& type);
	std::unique_ptr<AST::Node> parseDefFuncStatement(const TokenType& returnType = TokenType::UNKNOWN);

	std::unique_ptr<AST::AssignmentStatement> parseAssignment(std::vector<std::string> identifiers);
	std::unique_ptr<AST::Assignable> parseAssignable();
	std::unique_ptr<AST::ClassAssignment> parseClassAssignment();

	std::unique_ptr<AST::Parameter> parseParameter();

	std::unique_ptr<AST::Expression> parseExpression();
	std::unique_ptr<AST::Expression> parseTermExpression();
	std::unique_ptr<AST::Expression> parseFactorExpression();

	std::unique_ptr<AST::Node> parseCondition();
	std::unique_ptr<AST::Node> parseAndCondition();
	std::unique_ptr<AST::Node> parseRelationCondition();

	std::unique_ptr<AST::ReturnStatement> parseReturnStatement();

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