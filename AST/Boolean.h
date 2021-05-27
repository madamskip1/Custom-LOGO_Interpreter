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
        virtual void evaluate(Context* context) override;

	private:
		bool value;
	};
}
