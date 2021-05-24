#pragma once
#include "Node.h"
#include "Expression.h"

namespace AST
{
	class ClassAssignment :
		public AST::Node
	{
	public:
		ClassAssignment();

		const void addExpression(std::unique_ptr<AST::Expression> expression);

		std::size_t getExpressionsSize() const;
		AST::Expression* getExpression(int index = 0) const;

	private:
		std::vector<std::unique_ptr<AST::Expression>> expressions;
	};
}