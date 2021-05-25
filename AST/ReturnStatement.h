#pragma once
#include "Node.h"
#include "Assignable.h"

namespace AST
{
	class ReturnStatement
		: public AST::Node
	{
	public:
		ReturnStatement();
		ReturnStatement(std::unique_ptr<AST::Assignable> returnAssignable);

		virtual void execute(Context* context);

		AST::Assignable* getReturn() const;

	private:
		std::unique_ptr<AST::Assignable> returnAssignable;
	};
}