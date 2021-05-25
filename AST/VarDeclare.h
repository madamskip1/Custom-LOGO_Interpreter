#pragma once
#include <string>
#include "Node.h"
#include "ClassAssignment.h"
#include "AssignmentStatement.h"
#include "../include/TokenType.h"

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

		virtual void execute(Context* context);

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
