#pragma once
#include "Node.h"
#include "Assignable.h"

class ReturnStatement
	: public Node
{
public:
	ReturnStatement();

	const void setReturn(std::unique_ptr<Assignable> returnAssign);
	Assignable* getReturn() const;

private:
	std::unique_ptr<Assignable> returnAssignable;
};

