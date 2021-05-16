#pragma once
#include <vector>
#include <memory>

enum class NodeType
{
	NOTYPE,
	Root,
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
	AssignmentStatement,
	AssignClassStatement,
	VarDeclare,
	Number,
	Boolean,
	Variable,
	Color,
	ReturnStatement
};

class Node
{	
public:
	Node();
	Node(NodeType type);
	virtual ~Node() {};


	NodeType getNodeType() const;
	
	const void addChild(std::unique_ptr<Node> child);
	Node* getChild(const int& index) const;
	const std::size_t getChildrenSize() const;

protected:
	NodeType nodeType;

	std::vector<std::unique_ptr<Node>> children;
};

