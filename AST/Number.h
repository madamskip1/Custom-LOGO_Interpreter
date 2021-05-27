#pragma once
#include "Node.h"
#include "Expression.h"

namespace AST
{
	class Number :
		public AST::Expression
	{
	public:
		Number(int value);

        virtual void evaluate(Context* context) override;

		const int getValue() const;
	private:
		int value;
	};
}
