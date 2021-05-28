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
	};
}
