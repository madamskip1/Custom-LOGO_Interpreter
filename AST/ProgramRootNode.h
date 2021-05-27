#pragma once
#include <vector>
#include <memory>
#include "Node.h"
#include "../include/Context.h"

namespace AST
{
	class ProgramRootNode :
		public AST::Node
	{
	public:
		ProgramRootNode();

        virtual void execute(Context* context) override;
	};
}
