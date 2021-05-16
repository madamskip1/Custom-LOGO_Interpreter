#include "DefFuncStatement.h"

DefFuncStatement::DefFuncStatement() : Node(NodeType::DefFuncStatement) 
{
}

const void DefFuncStatement::setName(std::string name)
{
	this->name = name;
}

const void DefFuncStatement::setInstructionsBlock(std::unique_ptr<InstructionsBlock> block)
{
	instructionsBlock = std::move(block);
}

const void DefFuncStatement::addParameter(std::unique_ptr<Parameter> parameter)
{
	parameters.push_back(std::move(parameter));
}

const void DefFuncStatement::setReturnType(const TokenType& retType)
{
	returnType = retType;
}
std::string DefFuncStatement::getName() const
{
	return name;
}

InstructionsBlock* DefFuncStatement::getInstructionsBlock() const
{
	return instructionsBlock.get();
}

std::size_t DefFuncStatement::getParametersSize() const
{
	return parameters.size();
}

Parameter* DefFuncStatement::getParameter(const int& index) const
{
	return parameters[index].get();
}

const bool DefFuncStatement::hasReturnType() const
{
	return (returnType != TokenType::UNKNOWN);
}

TokenType DefFuncStatement::getReturnType() const
{
	return returnType;
}

