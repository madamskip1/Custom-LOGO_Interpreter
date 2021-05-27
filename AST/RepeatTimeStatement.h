#pragma once
#include <memory>
#include "Node.h"
#include "InstructionsBlock.h"
#include "Expression.h"

namespace AST
{
	class RepeatTimeStatement :
		public AST::Node
	{
	public:
		RepeatTimeStatement() = delete;
		RepeatTimeStatement(std::unique_ptr<AST::Expression> period, std::unique_ptr<AST::InstructionsBlock> block);
		RepeatTimeStatement(std::unique_ptr<AST::Expression> period, std::unique_ptr<AST::Expression> howManyTime, std::unique_ptr<AST::InstructionsBlock> block);

        virtual void execute(Context* context) override;

		AST::Expression* getPeriod() const;
		AST::Expression* getHowManyTime() const;
		AST::InstructionsBlock* getInstructuionsBlock() const;

	private:
		std::unique_ptr<AST::Expression> periodExpression = nullptr;
		std::unique_ptr<AST::Expression> howManyTimeExpression = nullptr;
		std::unique_ptr<AST::InstructionsBlock> instructionsBlock = nullptr;
	};
}
