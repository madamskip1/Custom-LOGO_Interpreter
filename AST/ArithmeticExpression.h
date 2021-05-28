#pragma once
#include <vector>
#include <memory>
#include "include/TokenType.h"
#include "Node.h"
#include "Assignable.h"
#include "Expression.h"

namespace AST
{
	class ArithmeticExpression :
        public AST::Expression
	{
	public:
		ArithmeticExpression();

        virtual void evaluate(Context* context) override;
        virtual bool isOnlyId() const override;
        virtual std::vector<std::string> getIdentifiers() const override;

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
