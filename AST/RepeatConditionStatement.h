#pragma once
#include <memory>
#include "Node.h"
#include "InstructionsBlock.h"
#include "Condition.h"
#include "../include/Context.h"

namespace AST
{
	class RepeatConditionStatement :
		public AST::Node
	{
	public:
		RepeatConditionStatement() = delete;
        RepeatConditionStatement(std::unique_ptr<AST::Node> condition, std::unique_ptr<InstructionsBlock> block);

        virtual void execute(Context* context) override;

	private:
        std::shared_ptr<AST::Node> conditionNode = nullptr;
        std::unique_ptr<AST::InstructionsBlock> instructionsBlock = nullptr;
	};
}
