#include "RepeatConditionStatement.h"
#include <thread>

AST::RepeatConditionStatement::RepeatConditionStatement(std::unique_ptr<AST::Node> condition, std::unique_ptr<InstructionsBlock> block) : AST::Node(AST::NodeType::RepeatConditionStatement)
{
	conditionNode = std::move(condition);
    instructionsBlock = std::move(block);
}

void AST::RepeatConditionStatement::execute(Context* context)
{
	
    bool whileCondition;
	conditionNode->evaluate(context);
	whileCondition = std::get<bool>(context->evaluateValue);

	while (whileCondition)
	{
        if (instructionsBlock->getChildrenSize() > 0)
        {
            instructionsBlock->execute(context);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
		
		conditionNode->evaluate(context);
		whileCondition = std::get<bool>(context->evaluateValue);
	}
}
