#include "Condition.h"
#include "Expression.h"
#include "Boolean.h"

Condition::Condition() : Node(NodeType::Condition)
{
}

bool Condition::evaluate()
{
	bool returnBoolean = false;

	if (leftCondition->getNodeType() == NodeType::Condition || leftCondition->getNodeType() == NodeType::Boolean)
	{
		if (leftCondition->getNodeType() == NodeType::Condition)
		{
			returnBoolean = (static_cast<Condition*>(leftCondition.get()))->evaluate();
		}
		else
		{
			returnBoolean = (static_cast<Boolean*>(leftCondition.get()))->evaluate();
		}


		if (rightCondition)
		{
			if (rightCondition->getNodeType() == NodeType::Condition || rightCondition->getNodeType() == NodeType::Boolean)
			{
				bool rightBoolean;
				if (rightCondition->getNodeType() == NodeType::Condition)
				{
					rightBoolean = (static_cast<Condition*>(rightCondition.get()))->evaluate();
				}
				else
				{
					rightBoolean = (static_cast<Boolean*>(rightCondition.get()))->evaluate();
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
	else if (leftCondition->getNodeType() == NodeType::Expression)
	{
		int leftValue = (static_cast<Expression*>(leftCondition.get()))->evaluate();

		if (rightCondition->getNodeType() == NodeType::Expression)
		{
			int rightValue = (static_cast<Expression*>(rightCondition.get()))->evaluate();

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

const void Condition::setLeftCondition(std::unique_ptr<Node> condition)
{
	leftCondition = std::move(condition);
}

const void Condition::setRightCondition(std::unique_ptr<Node> condition)
{
	rightCondition = std::move(condition);
}

const void Condition::setRelationOperator(const TokenType& relOp)
{
	relationOperator = relOp;
}

const void Condition::setNotOperator(const bool& notOp)
{
	notOperator = notOp;
}

Node* Condition::getLeftCondition() const
{
	return leftCondition.get();
}

Node* Condition::getRightCondition() const
{
	return rightCondition.get();
}

const bool Condition::getNotOperator() const
{
	return notOperator;
}

const TokenType Condition::getRelationOperator() const
{
	return relationOperator;
}
