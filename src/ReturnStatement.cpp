#include "ReturnStatement.h"

AST::ReturnStatement::ReturnStatement() : AST::Node(AST::NodeType::ReturnStatement)
{
	returnAssignable = nullptr;
}

AST::ReturnStatement::ReturnStatement(std::unique_ptr<AST::Assignable> returnAssignable) : AST::Node(AST::NodeType::ReturnStatement)
{
	this->returnAssignable = std::move(returnAssignable);
}

AST::Assignable* AST::ReturnStatement::getReturn() const
{
	return returnAssignable.get();
}
