#pragma once
#include <memory>

enum class NodeType
{
	NOTYPE,
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
	RelationCondition,
	AssignStatement,
	AssignClassStatement,
	DeclareVarStatement,

};

class Node
{	
public:
	Node() :nodeType(NodeType::NOTYPE) {};
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

