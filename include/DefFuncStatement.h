#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Node.h"
#include "InstructionsBlock.h"
#include "Parameter.h"
#include "TokenType.h"

class DefFuncStatement :
	public Node
{
public:
	DefFuncStatement();

	const void setName(const std::string name);
	const void setInstructionsBlock(std::unique_ptr<InstructionsBlock> block);
	const void addParameter(std::unique_ptr<Parameter> parameter);
	const void setReturnType(const TokenType& retType);

	std::string getName() const;
	InstructionsBlock* getInstructionsBlock() const;
	std::size_t getParametersSize() const;
	Parameter* getParameter(const int& index) const;

	const bool hasReturnType() const;
	TokenType getReturnType() const;

private:
	std::string name = "";
	std::unique_ptr<InstructionsBlock> instructionsBlock = nullptr;
	std::vector<std::unique_ptr<Parameter>> parameters;
	TokenType returnType = TokenType::UNKNOWN;
};
