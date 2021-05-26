#pragma once
#include "Condition.h"
#include "Assignable.h"

namespace AST
{
	class Boolean :
		public AST::Assignable
	{
	public:
		Boolean(bool val);

		const bool getValue() const;
		bool evaluate() const;
        int evaluate(Context* context) const;
	private:
		bool value;
	};
}
