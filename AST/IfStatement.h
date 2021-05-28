#pragma once

#include "Node.h"
#include "InstructionsBlock.h"
#include "Expression.h"
#include "../include/Context.h"

namespace AST
{
	class IfStatement :
		public AST::Node
	{
	public:
		IfStatement() = delete;
        IfStatement(std::unique_ptr<AST::Expression> condition, std::unique_ptr <AST::InstructionsBlock> trueBlock, std::unique_ptr <AST::InstructionsBlock> elseBlock = nullptr);

        virtual void execute(Context* context) override;

		const bool hasElseBlock() const;
		AST::InstructionsBlock* getTrueBlockNode() const;
		AST::InstructionsBlock* getElseBlockNode() const;
        AST::Expression* getCondition() const;

	private:
        std::unique_ptr<AST::Expression> conditionNode = nullptr;
		std::unique_ptr<AST::InstructionsBlock> trueBlockNode = nullptr;
		std::unique_ptr<AST::InstructionsBlock> elseBlockNode = nullptr;
	};
}
