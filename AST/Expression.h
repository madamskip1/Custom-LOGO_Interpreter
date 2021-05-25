#pragma once
#include <vector>
#include <memory>
#include "include/TokenType.h"
#include "AST/Node.h"
#include "Assignable.h"

namespace AST
{
	class Expression :
		public AST::Assignable
	{
	public:
		Expression();
        virtual int evaluate(Context* context) const;

		const void addChildExpression(std::unique_ptr<AST::Expression> child);
		const void addOperator(const TokenType& op);
		const void setNegativeOp(const bool& negative);
		const std::size_t getChildrenExpressionSize() const;

		AST::Expression* getChildExpression(const int& index) const;
		const bool getNegativeOperator() const;
		const TokenType getOperator(const int& index) const;

	protected:
		bool negativeOperator = false;
		std::vector<std::unique_ptr<AST::Expression>> childrenExpressions;
		std::vector<TokenType> operators;
	};
}
