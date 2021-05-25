#pragma once
#include <memory>
#include "Node.h"
#include "InstructionsBlock.h"
#include "Expression.h"
#include "../include/Context.h"

namespace AST
{
	class RepeatStatement :
		public AST::Node
	{
	public:
		RepeatStatement() = delete;
		RepeatStatement(std::unique_ptr<AST::Expression> howManyTime, std::unique_ptr<AST::InstructionsBlock> block);

		virtual void execute(Context* context);

		AST::Expression* getHowManyTime() const;
		AST::InstructionsBlock* getInstructuionsBlock() const;

	private:
		std::shared_ptr<AST::Expression> howManyTimeExpression = nullptr;
		std::unique_ptr<AST::InstructionsBlock> instructionsBlock = nullptr;
	};
}
