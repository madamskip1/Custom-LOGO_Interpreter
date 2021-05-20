#pragma once
#include "Node.h"
#include "Assignable.h"

class ReturnStatement
	: public Node
{
public:
	ReturnStatement();
	ReturnStatement(std::unique_ptr<Assignable> returnAssignable);

	Assignable* getReturn() const;

private:
	std::unique_ptr<Assignable> returnAssignable;
};

