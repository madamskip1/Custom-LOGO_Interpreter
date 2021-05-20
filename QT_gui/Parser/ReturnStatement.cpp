#include "ReturnStatement.h"

ReturnStatement::ReturnStatement() : Node(NodeType::ReturnStatement)
{
	returnAssignable = nullptr;
}

ReturnStatement::ReturnStatement(std::unique_ptr<Assignable> returnAssignable) : Node(NodeType::ReturnStatement)
{
	this->returnAssignable = std::move(returnAssignable);
}

Assignable* ReturnStatement::getReturn() const
{
	return returnAssignable.get();
}
