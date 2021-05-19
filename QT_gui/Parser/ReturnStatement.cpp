#include "ReturnStatement.h"

ReturnStatement::ReturnStatement() : Node(NodeType::ReturnStatement)
{
}

const void ReturnStatement::setReturn(std::unique_ptr<Assignable> returnAssign)
{
	returnAssignable = std::move(returnAssign);
}

Assignable* ReturnStatement::getReturn() const
{
	return returnAssignable.get();
}
