#include "IfStatement.h"
#include <iostream>
#include <QDebug>

AST::IfStatement::IfStatement(std::unique_ptr<AST::Node> condition, std::unique_ptr<AST::InstructionsBlock> trueBlock, std::unique_ptr<AST::InstructionsBlock> elseBlock) : AST::Node(AST::NodeType::IfStatement)
{
	conditionNode = std::move(condition);
	trueBlockNode = std::move(trueBlock);
	if (elseBlock)
		elseBlockNode = std::move(elseBlock);
}

void AST::IfStatement::execute(Context* context)
{
    conditionNode->evaluate(context);
    bool ifCondition = std::get<bool>(context->evaluateValue);

	if (ifCondition)
	{
		trueBlockNode->execute(context);
	}
	else if (elseBlockNode)
	{
		elseBlockNode->execute(context);
	}
}

const bool AST::IfStatement::hasElseBlock() const
{
	return elseBlockNode != nullptr;
}

AST::InstructionsBlock* AST::IfStatement::getTrueBlockNode() const
{
	return trueBlockNode.get();
}

AST::InstructionsBlock* AST::IfStatement::getElseBlockNode() const
{
	return elseBlockNode.get();
}

AST::Node* AST::IfStatement::getCondition() const
{
	return conditionNode.get();
}
