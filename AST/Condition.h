#pragma once
#include <memory>
#include "Node.h"
#include "include/TokenType.h"

namespace AST
{
	class Condition :
		public AST::Node
	{
	public:
		Condition();

        virtual bool evaluate(Context* context);
	   
		const void setLeftCondition(std::unique_ptr<AST::Node> condition);
		const void setRightCondition(std::unique_ptr<AST::Node> condition);
		const void setNotOperator(const bool& notOp);
		const void setRelationOperator(const TokenType& relOp);

		Node* getLeftCondition() const;
		Node* getRightCondition() const;
		const bool getNotOperator() const;
		const TokenType getRelationOperator() const;

	private:
		std::unique_ptr<AST::Node> leftCondition = nullptr;
		std::unique_ptr<AST::Node> rightCondition = nullptr;
		TokenType relationOperator;
		bool notOperator;
	};
}
