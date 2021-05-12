#pragma once
#include <string>
#include <memory>
#include <vector>
#include "InstructionsBlock.h"
#include "Parameter.h"
#include "TokenType.h"

class DefFuncStatement :
	public Node
{
public:
	DefFuncStatement();

	const void setName(const std::string name);
	const void setInstructionsBlock(std::shared_ptr<InstructionsBlock> block);
	const void addParameter(std::shared_ptr<Parameter> parameter);
	const void setReturnType(const TokenType& retType);

	std::string getName() const;
	std::shared_ptr<InstructionsBlock> getInstructionsBlock() const;
	std::size_t getParametersSize() const;
	std::shared_ptr<Parameter> getParameter(const int& index) const;

	const bool hasReturnType() const;
	TokenType getReturnType() const;

private:
	std::string name = "";
	std::shared_ptr<InstructionsBlock> instructionsBlock = nullptr;
	std::vector<std::shared_ptr<Parameter>> parameters;
	TokenType returnType = TokenType::UNKNOWN;
};

