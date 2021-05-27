#pragma once
#include <vector>
#include <memory>
#include "AST/Node.h"


namespace AST
{
	class InstructionsBlock :
		public AST::Node
	{
	public:
		InstructionsBlock();
        virtual void execute(Context* context) override;
	private:
	};
}
