#include "Condition.h"
#include "Expression.h"
#include "Boolean.h"
#include "../include/Context.h"

AST::Condition::Condition()
{
    nodeType = AST::NodeType::Condition;
}

void AST::Condition::evaluate(Context* context)
{
    std::variant<std::monostate, int, std::string, bool, Variable*> curValue;

    bool returnBoolean = false;
    childrenExpressions[0]->evaluate(context);
    curValue = context->evaluateValue;

    if (curValue.index() == 3)
    {
        returnBoolean = std::get<bool>(context->evaluateValue);

        if (childrenExpressions.size() == 2)
        {
            childrenExpressions[1]->evaluate((context));
            bool rightBoolean = std::get<bool>(context->evaluateValue);

            if (relationOperator == TokenType::And)
            {
                returnBoolean = (returnBoolean && rightBoolean);
            }
            else
            {
                returnBoolean = (returnBoolean || rightBoolean);
            }
        }

    }
    else if (curValue.index() == 1)
	{
        int leftValue = std::get<int>(context->evaluateValue);

        if (childrenExpressions.size() == 2)
        {
            childrenExpressions[1]->evaluate((context));
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
            return;
        }
	}
    else if (curValue.index() == 2 || curValue.index() == 4 )
    {
        return;
    }
	else
	{
        context->evaluateValue = false;
        return;
	}

    if (negativeOperator)
		returnBoolean = !returnBoolean;

    context->evaluateValue = returnBoolean;
}


const void AST::Condition::setRelationOperator(const TokenType& relOp)
{
	relationOperator = relOp;
}


const TokenType AST::Condition::getRelationOperator() const
{
    return relationOperator;
}

bool AST::Condition::evaluateConditionOrBoolean(Node* node, Context *context)
{
    node->evaluate(context);
    return std::get<bool>(context->evaluateValue);
}
