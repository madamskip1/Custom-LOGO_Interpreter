#pragma once
#include <memory>
#include "Node.h"
#include "Expression.h"
#include "include/TokenType.h"

namespace AST
{
	class Condition :
        public AST::Expression
	{
	public:
		Condition();

        virtual void evaluate(Context* context);

        const void setRelationOperator(const TokenType& relOp);
		const TokenType getRelationOperator() const;

	private:
		TokenType relationOperator;

        bool evaluateConditionOrBoolean(Node* node, Context* context = nullptr);
	};
}
