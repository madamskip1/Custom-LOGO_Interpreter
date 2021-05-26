#include "DefFuncStatement.h"
#include "QDebug"

AST::DefFuncStatement::DefFuncStatement() : Node(AST::NodeType::DefFuncStatement) 
{
}

AST::DefFuncStatement::~DefFuncStatement()
{
    qDebug() << "DUPADEFF FUNC";
}

void AST::DefFuncStatement::executeFunction(Context* context)
{
    // TODO: set args to context

    instructionsBlock->execute(context);
}

const void AST::DefFuncStatement::setName(std::string name)
{
	this->name = name;
}

const void AST::DefFuncStatement::setInstructionsBlock(std::unique_ptr<AST::InstructionsBlock> block)
{
	instructionsBlock = std::move(block);
}

const void AST::DefFuncStatement::addParameter(std::unique_ptr<AST::Parameter> parameter)
{
	parameters.push_back(std::move(parameter));
}

const void AST::DefFuncStatement::setReturnType(const TokenType& retType)
{
	returnType = retType;
}
std::string AST::DefFuncStatement::getName() const
{
	return name;
}

AST::InstructionsBlock* AST::DefFuncStatement::getInstructionsBlock() const
{
	return instructionsBlock.get();
}

std::size_t AST::DefFuncStatement::getParametersSize() const
{
	return parameters.size();
}

AST::Parameter* AST::DefFuncStatement::getParameter(const int& index) const
{
	return parameters[index].get();
}

const bool AST::DefFuncStatement::hasReturnType() const
{
	return (returnType != TokenType::UNKNOWN);
}

TokenType AST::DefFuncStatement::getReturnType() const
{
	return returnType;
}

