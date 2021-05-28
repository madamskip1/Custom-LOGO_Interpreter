#include "RepeatConditionStatement.h"

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
		instructionsBlock->execute(context);
		
		conditionNode->evaluate(context);
		whileCondition = std::get<bool>(context->evaluateValue);
	}
}
