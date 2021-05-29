#pragma once
#include <vector>
#include <memory>
//#include "Context.h"
#include "Token.h"

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
        RepeatConditionStatement,
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
        virtual void execute(Context*) {};
        virtual void evaluate(Context*) {};

		NodeType getNodeType() const;
		
        void addChild(std::unique_ptr<AST::Node> child);
		AST::Node* getChild(const int& index) const;
        std::size_t getChildrenSize() const;
        void releaseChild(const int& index);

        void setToken(Token token);

	protected:
		AST::NodeType nodeType;
        Token token;

		std::vector<std::unique_ptr<AST::Node>> children;
	};
}
