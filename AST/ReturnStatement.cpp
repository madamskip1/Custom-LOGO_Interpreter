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
	TokenType type = context->returnType;
	if (type == TokenType::Integer)
	{
		Expression* number = static_cast<Expression*>(returnAssignable.get());
        number->evaluate(context);
        context->returnVariant = context->evaluateValue;
	}
	else if (type == TokenType::Boolean)
	{
		Boolean* boolean = static_cast<Boolean*>(returnAssignable.get());
		context->returnVariant = boolean->evaluate();
	}
}

AST::Assignable* AST::ReturnStatement::getReturn() const
{
	return returnAssignable.get();
}
