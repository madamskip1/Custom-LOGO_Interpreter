#include "TermExpression.h"

TermExpression::TermExpression() : Node(NodeType::ExpressionTerm) {};

const void TermExpression::addExpressionFactor(std::shared_ptr<FactorExpression> factor)
{
	expressionFactors.push_back(factor);
}

const void TermExpression::addMutliOperartor(const TokenType& multiOperator)
{
	multiOperators.push_back(multiOperator);
}

const void TermExpression::addNextExpressionFactor(std::shared_ptr<FactorExpression> factor, const TokenType& multiOperator)
{
	expressionFactors.push_back(factor);
	multiOperators.push_back(multiOperator);
}

const std::size_t TermExpression::getFactorsSize() const
{
	return expressionFactors.size();
}

std::shared_ptr<FactorExpression> TermExpression::getExpressionFactor(const int& index) const
{
	return expressionFactors[index];
}

TokenType TermExpression::getOperator(const int& index) const
{
	return multiOperators[index];
}
