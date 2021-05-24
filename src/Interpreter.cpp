#include "Interpreter.h"
#include "Node.h"

Interpreter::Interpreter(std::unique_ptr<AST::ProgramRootNode> rootNode)
{
	programNode = std::move(rootNode);
	context = std::make_unique<Context>();
}

void Interpreter::run()
{
	lookForDefFunctions();

	int rootNodeChildrenSize = programNode->getChildrenSize();
	for (int i = 0; i < rootNodeChildrenSize; i++)
	{

	}
}

void Interpreter::lookForDefFunctions()
{
	int rootNodeChildrenSize = programNode->getChildrenSize();
	
	for (int i = 0; i < rootNodeChildrenSize; i++)
	{
		AST::Node* curNode = programNode->getChild(0);
		if (curNode->getNodeType() == AST::NodeType::DefFuncStatement)
		{
			AST::DefFuncStatement* defFuncNode = static_cast<AST::DefFuncStatement*>(curNode);
			context->addDefFunction(defFuncNode);
		}
	}
}
