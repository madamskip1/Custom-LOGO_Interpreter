#include "DefFuncStatement.h"

const void DefFuncStatement::setName(std::string name)
{
	this->name = name;
}

const void DefFuncStatement::setInstructionsBlock(std::shared_ptr<InstructionsBlock> block)
{
	instructionsBlock = block;
}

const void DefFuncStatement::addParameter(std::shared_ptr<Parameter> parameter)
{
	parameters.push_back(parameter);
}

const void DefFuncStatement::setReturnType(TokenType retType)
{
	returnType = retType;
}

std::string DefFuncStatement::getName() const
{
	return name;
}

std::shared_ptr<InstructionsBlock> DefFuncStatement::getInstructionsBlock() const
{
	return instructionsBlock;
}

std::size_t DefFuncStatement::getParametersSize() const
{
	return parameters.size();
}

std::shared_ptr<Parameter> DefFuncStatement::getParameter(const int& index) const
{
	return parameters[index];
}

bool DefFuncStatement::hasReturnType() const
{
	return (returnType != TokenType::UNKNOWN);
}

TokenType DefFuncStatement::getReturnType() const
{
	return returnType;
}
