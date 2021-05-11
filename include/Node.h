#pragma once
#include <memory>

enum class NodeType
{
	IfStatement,
	RepeatStatement,
	RepeatTimeStatement,
	InstructionsBlock,
	Expression,
	ExpressionTerm,
	ExpressionFactor,
	DefFuncStatement,
	Parameter,
	CallFuncStatement,
	Condition,
	AndCondition,
	RelationCondition
};

class Node
{	
public:
	Node(NodeType type) : nodeType(type) {};

	void setParentNode(std::shared_ptr<Node> node);
	NodeType getNodeType() {
		return nodeType;
	}
protected:
	NodeType nodeType;
private:
	std::weak_ptr<Node> parentNode;
	
};

