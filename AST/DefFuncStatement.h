#pragma once
#include <memory>
#include <vector>
#include <string>
#include "../AST/Node.h"
#include "../AST/InstructionsBlock.h"
#include "../AST/Parameter.h"
#include "../AST/Assignable.h"
#include "include/TokenType.h"

namespace AST
{
	class DefFuncStatement :
		public AST::Node
	{
	public:
		DefFuncStatement();
		
		void executeFunction(Context* context);

		const void setName(const std::string name);
		const void setInstructionsBlock(std::unique_ptr<AST::InstructionsBlock> block);
		const void addParameter(std::unique_ptr<AST::Parameter> parameter);
		const void setReturnType(const TokenType& retType);

		std::string getName() const;
		AST::InstructionsBlock* getInstructionsBlock() const;
		std::size_t getParametersSize() const;
		AST::Parameter* getParameter(const int& index) const;

		const bool hasReturnType() const;
		TokenType getReturnType() const;

	private:
		std::string name = "";
		std::unique_ptr<AST::InstructionsBlock> instructionsBlock = nullptr;
		std::vector<std::unique_ptr<AST::Parameter>> parameters;
		TokenType returnType = TokenType::UNKNOWN;
	};
}
