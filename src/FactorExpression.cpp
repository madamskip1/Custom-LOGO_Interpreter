#include "FactorExpression.h"

FactorExpression::FactorExpression() : Node(NodeType::ExpressionFactor) {};

const void FactorExpression::setNegativeOp(const bool & negative)
{
	negativeOp = negative;
}

const void FactorExpression::setExpression(std::shared_ptr<Expression> expr)
{
	expression = expr;
}

const void FactorExpression::setIntVal(const int& val)
{
	intVal = val;
}

const void FactorExpression::setCallFunc(std::shared_ptr<CallFuncStatement> func)
{
	callFunc = func;
}

const void FactorExpression::setVariable(std::vector<std::string> varName)
{
	variable = varName;
}

int FactorExpression::getIntVal() const
{
	return intVal;
}

const bool FactorExpression::getNegativeOp() const
{
	return negativeOp;
}

std::shared_ptr<Expression> FactorExpression::getExpression() const
{
	return expression;
}

std::shared_ptr<CallFuncStatement> FactorExpression::getCallFunc() const
{
	return callFunc;
}

std::vector<std::string> FactorExpression::getVarName() const
{
	return variable;
}
