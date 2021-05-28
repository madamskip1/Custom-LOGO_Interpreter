#pragma once
#include <vector>
#include <memory>
//#include "Context.h"

class Context;

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
        ArithmeticExpression,
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
        ReturnStatement,
        DeleteStatement
	};

	class Node
	{
	public:
		Node();
		Node(AST::NodeType type);
		virtual ~Node() {};

		virtual void execute() {};
		virtual void execute(Context* context) {};

		NodeType getNodeType() const;
		
		const void addChild(std::unique_ptr<AST::Node> child);
		AST::Node* getChild(const int& index) const;
		const std::size_t getChildrenSize() const;
        void releaseChild(const int& index);

	protected:
		AST::NodeType nodeType;

		std::vector<std::unique_ptr<AST::Node>> children;
	};
}
