#pragma once
#include <vector>
#include <memory>
#include "Node.h"

namespace AST
{
	class ProgramRootNode :
		public AST::Node
	{
	public:
		ProgramRootNode();
	};
}