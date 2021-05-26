#pragma once
#include "Node.h"

namespace AST 
{
	class Assignable :
		public AST::Node
	{
	public:
        virtual bool isOnlyId() const { return false; }
        virtual int evaluate(Context*) const { return 0; };
	};

}

