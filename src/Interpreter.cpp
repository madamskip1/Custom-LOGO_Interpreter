#include "Interpreter.h"
#include "../AST/Node.h"
#include "../AST/DefFuncStatement.h"

Interpreter::Interpreter(std::unique_ptr<AST::ProgramRootNode> rootNode, DrawingBoard* drawingBoard, TurtleBoard* turtleBoard)
{
	programNode = std::move(rootNode);
    context = std::make_unique<Context>();
    context->setDrawingBoard(drawingBoard);
    context->setTurtleBoard(turtleBoard);
}


void Interpreter::run()
{
	lookForDefFunctions();

	std::size_t rootNodeChildrenSize = programNode->getChildrenSize();
	for (int i = 0; i < rootNodeChildrenSize; i++)
	{
		AST::Node* curNode = programNode->getChild(i);
		if (curNode->getNodeType() != AST::NodeType::DefFuncStatement)
		{
			curNode->execute(context.get());
		}
	}
}

void Interpreter::lookForDefFunctions()
{
	std::size_t rootNodeChildrenSize = programNode->getChildrenSize();
	
	for (int i = 0; i < rootNodeChildrenSize; i++)
	{
		AST::Node* curNode = programNode->getChild(i);
		if (curNode->getNodeType() == AST::NodeType::DefFuncStatement)
		{
			AST::DefFuncStatement* defFuncNode = static_cast<AST::DefFuncStatement*>(curNode);
			context->addDefFunction(defFuncNode);
		}
	}
}

/*

  Point point(20, 30);
  Turtle zolw(point);
 * */
