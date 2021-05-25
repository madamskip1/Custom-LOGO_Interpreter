#pragma once
#include "Node.h"

namespace AST 
{
	class Assignable :
		public AST::Node
	{
	public:
        virtual bool isOnlyId(Context* context) const { return false; }
	};

}

