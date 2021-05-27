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

        int evaluate(Context* context) const override;

		const int getValue() const;
	private:
		int value;
	};
}
