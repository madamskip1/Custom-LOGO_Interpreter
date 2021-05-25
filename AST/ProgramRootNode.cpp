#include "ProgramRootNode.h"

AST::ProgramRootNode::ProgramRootNode() : Node(AST::NodeType::Root)
{
}

void AST::ProgramRootNode::execute(Context* context)
{
	for (auto& node : children)
	{
		if (node->getNodeType() != AST::NodeType::DefFuncStatement)
		{
			node->execute(context);
		}
	}
}
