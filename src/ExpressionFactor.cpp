#include "ExpressionFactor.h"

const void ExpressionFactor::setNegativeOp(const bool & negative)
{
	negativeOp = negative;
}

const void ExpressionFactor::setExpression(std::shared_ptr<Expression> expr)
{
	expression = expr;
}

const void ExpressionFactor::setIntVal(const int& val)
{
	intVal = val;
}

int ExpressionFactor::getIntVal() const
{
	return intVal;
}

bool ExpressionFactor::getNegativeOp() const
{
	return negativeOp;
}

std::shared_ptr<Expression> ExpressionFactor::getExpression() const
{
	return expression;
}
