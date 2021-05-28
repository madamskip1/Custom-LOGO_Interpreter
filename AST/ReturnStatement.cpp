#include "ReturnStatement.h"
#include "Boolean.h"
#include "Number.h"
#include "../include/Context.h"

AST::ReturnStatement::ReturnStatement() : AST::Node(AST::NodeType::ReturnStatement)
{
	returnAssignable = nullptr;
}

AST::ReturnStatement::ReturnStatement(std::unique_ptr<AST::Assignable> returnAssignable) : AST::Node(AST::NodeType::ReturnStatement)
{
	this->returnAssignable = std::move(returnAssignable);
}

void AST::ReturnStatement::execute(Context* context)
{
    if (returnAssignable)
    {
        returnAssignable->evaluate(context);
        context->returnVariant = context->evaluateValue;
    }
}

AST::Assignable* AST::ReturnStatement::getReturn() const
{
	return returnAssignable.get();
}
