#include "Interpreter.h"
#include "../AST/Node.h"
#include "../AST/DefFuncStatement.h"

Interpreter::Interpreter(std::unique_ptr<AST::ProgramRootNode> rootNode, Context* context) : programNode(std::move(rootNode)), context(context)
{}


void Interpreter::run()
{
	lookForDefFunctions();

	std::size_t rootNodeChildrenSize = programNode->getChildrenSize();
    for (std::size_t i = 0; i < rootNodeChildrenSize; i++)
	{
		AST::Node* curNode = programNode->getChild(i);
        if (curNode)
		{
            curNode->execute(context);
		}
	}
}

void Interpreter::lookForDefFunctions()
{
	std::size_t rootNodeChildrenSize = programNode->getChildrenSize();

    for (std::size_t i = 0; i < rootNodeChildrenSize; i++)
	{
		AST::Node* curNode = programNode->getChild(i);
		if (curNode->getNodeType() == AST::NodeType::DefFuncStatement)
		{
			AST::DefFuncStatement* defFuncNode = static_cast<AST::DefFuncStatement*>(curNode);
			context->addDefFunction(defFuncNode);
            programNode->releaseChild(i);
		}
	}

}

