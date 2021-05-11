#include "ExpressionTerm.h"

void ExpressionTerm::addExpressionFactor(std::shared_ptr<ExpressionFactor> factor)
{
	expressionFactors.push_back(factor);
}

void ExpressionTerm::addMutliOperartor(TokenType multiOperator)
{
	multiOperators.push_back(multiOperator);
}

void ExpressionTerm::addNextExpressionFactor(std::shared_ptr<ExpressionFactor> factor, TokenType multiOperator)
{
	expressionFactors.push_back(factor);
	multiOperators.push_back(multiOperator);
}

std::size_t ExpressionTerm::getFactorsSize()
{
	return expressionFactors.size();
}

std::shared_ptr<ExpressionFactor> ExpressionTerm::getExpressionFactor(int index)
{
	return expressionFactors[index];
}

TokenType ExpressionTerm::getOperator(int index)
{
	return multiOperators[index];
}
