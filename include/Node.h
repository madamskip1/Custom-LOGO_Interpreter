#pragma once
#include <vector>
#include <memory>

namespace AST
{
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
		Node(AST::NodeType type);
		virtual ~Node() {};


		NodeType getNodeType() const;
		
		const void addChild(std::unique_ptr<AST::Node> child);
		AST::Node* getChild(const int& index) const;
		const std::size_t getChildrenSize() const;

	protected:
		AST::NodeType nodeType;

		std::vector<std::unique_ptr<AST::Node>> children;
	};
}