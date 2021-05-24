#pragma once
#include <string>
#include "Node.h"
#include "TokenType.h"
#include "ClassAssignment.h"
#include "AssignmentStatement.h"

namespace AST 
{
	class VarDeclare :
		public AST::Node
	{
	public:
		VarDeclare() = delete;
		VarDeclare(TokenType type, std::string identifier);
		VarDeclare(TokenType type, std::string identifier, std::unique_ptr<AST::AssignmentStatement> assignStatement);
		VarDeclare(TokenType type, std::string identifier, std::unique_ptr<AST::ClassAssignment> classAssign);

		TokenType getType() const;
		std::string getIdentifier() const;

		AST::ClassAssignment* getClassAssignment() const;
		AST::AssignmentStatement* getAssignment() const;

	private:
		TokenType type = TokenType::UNKNOWN;
		std::string identifier;
		std::unique_ptr<AST::ClassAssignment> classAssignment;
		std::unique_ptr<AST::AssignmentStatement> assignment;
	};
}