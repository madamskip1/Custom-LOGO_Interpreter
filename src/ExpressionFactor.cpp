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
