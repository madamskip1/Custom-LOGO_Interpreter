#include "InstructionsBlock.h"
#include "../include/Context.h"

AST::InstructionsBlock::InstructionsBlock() : AST::Node(AST::NodeType::InstructionsBlock)
{
}

void AST::InstructionsBlock::execute(Context* context)
{
	context->addNewScope();

	for (const auto& node : children)
	{
		node->execute(context);
	}

	context->removeScope();
}
