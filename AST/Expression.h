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

        virtual void evaluate(Context*) override {};
        virtual bool isOnlyId() const override;
        virtual std::vector<std::string> getIdentifiers() const;

        void addChildExpression(std::unique_ptr<AST::Expression> child);
        std::size_t getChildrenExpressionSize() const;

        void setNegativeOp(const bool& negative);
        void addOperator(const TokenType& op);

		AST::Expression* getChildExpression(const int& index) const;
        bool getNegativeOperator() const;
        TokenType getOperator(const int& index) const;

	protected:
		bool negativeOperator = false;
		std::vector<std::unique_ptr<AST::Expression>> childrenExpressions;
		std::vector<TokenType> operators;
	};
}
