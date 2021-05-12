#pragma once

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
	AssignStatement,
	AssignClassStatement,
	DeclareVarStatement,

};

class Node
{	
public:
	Node();
	Node(NodeType type);

	NodeType getNodeType() const;

protected:
	NodeType nodeType;
};

