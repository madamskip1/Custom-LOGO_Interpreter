#include "Condition.h"
#include "Expression.h"
#include "Boolean.h"
#include "../include/Context.h"

AST::Condition::Condition() : AST::Node(AST::NodeType::Condition)
{
}

bool AST::Condition::evaluate(Context* context)
{
	bool returnBoolean = false;

	if (leftCondition->getNodeType() == NodeType::Condition || leftCondition->getNodeType() == AST::NodeType::Boolean)
	{
		if (leftCondition->getNodeType() == AST::NodeType::Condition)
		{
            returnBoolean = (static_cast<AST::Condition*>(leftCondition.get()))->evaluate(context);
		}
		else
		{
            returnBoolean = (static_cast<AST::Boolean*>(leftCondition.get()))->evaluate();
		}


		if (rightCondition)
		{
			if (rightCondition->getNodeType() == AST::NodeType::Condition || rightCondition->getNodeType() == AST::NodeType::Boolean)
			{
				bool rightBoolean;
				if (rightCondition->getNodeType() == AST::NodeType::Condition)
				{
                    rightBoolean = (static_cast<AST::Condition*>(rightCondition.get()))->evaluate(context);
				}
				else
				{
                    rightBoolean = (static_cast<AST::Boolean*>(rightCondition.get()))->evaluate();
				}

				
				if (relationOperator == TokenType::And)
				{
					returnBoolean = (returnBoolean && rightBoolean);
				}
				else
				{
					returnBoolean = (returnBoolean || rightBoolean);
				}
			}
			else
			{
				return false;
			}
		}
	}
	else if (leftCondition->getNodeType() == AST::NodeType::Expression)
	{
        (static_cast<AST::Expression*>(leftCondition.get()))->evaluate(context);
        int leftValue = std::get<int>(context->evaluateValue);

		if (rightCondition->getNodeType() == AST::NodeType::Expression)
		{
            (static_cast<AST::Expression*>(rightCondition.get()))->evaluate(context);
            int rightValue = std::get<int>(context->evaluateValue);

			if (relationOperator == TokenType::Equal)
				returnBoolean = (leftValue == rightValue);
			else if (relationOperator == TokenType::NotEqual)
				returnBoolean = (leftValue != rightValue);
			else if (relationOperator == TokenType::Less)
				returnBoolean = (leftValue < rightValue);
			else if (relationOperator == TokenType::Greater)
				returnBoolean = (leftValue > rightValue);
			else if (relationOperator == TokenType::LessOrEqual)
				returnBoolean = (leftValue <= rightValue);
			else if (relationOperator == TokenType::GreaterOrEqual)
				returnBoolean = (leftValue >= rightValue);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	if (notOperator)
		returnBoolean = !returnBoolean;

	return returnBoolean;
}

const void AST::Condition::setLeftCondition(std::unique_ptr<AST::Node> condition)
{
	leftCondition = std::move(condition);
}

const void AST::Condition::setRightCondition(std::unique_ptr<AST::Node> condition)
{
	rightCondition = std::move(condition);
}

const void AST::Condition::setRelationOperator(const TokenType& relOp)
{
	relationOperator = relOp;
}

const void AST::Condition::setNotOperator(const bool& notOp)
{
	notOperator = notOp;
}

AST::Node* AST::Condition::getLeftCondition() const
{
	return leftCondition.get();
}

AST::Node* AST::Condition::getRightCondition() const
{
	return rightCondition.get();
}

const bool AST::Condition::getNotOperator() const
{
	return notOperator;
}

const TokenType AST::Condition::getRelationOperator() const
{
	return relationOperator;
}
