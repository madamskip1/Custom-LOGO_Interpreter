#pragma once
#include "Condition.h"
#include "Assignable.h"
#include "Expression.h"

namespace AST
{
	class Boolean :
        public AST::Condition
	{
	public:
		Boolean(bool val);

		const bool getValue() const;
		bool evaluate() const;
        virtual void evaluate(Context* context) override;

	private:
		bool value;
	};
}
