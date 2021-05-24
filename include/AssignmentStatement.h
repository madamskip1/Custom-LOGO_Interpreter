#pragma once
#include <vector>
#include <string>
#include "Node.h"
#include "Assignable.h"

namespace AST 
{
	class AssignmentStatement :
		public AST::Node
	{
	public:
		AssignmentStatement() = delete;
		AssignmentStatement(std::vector<std::string> ids, std::unique_ptr<AST::Assignable> assignable);

		std::string getIdentifier(int index = 0) const;

		AST::Assignable* getAssign() const;

	private:
		std::vector<std::string> identifiers;
		std::unique_ptr<AST::Assignable> assign;
	};
}