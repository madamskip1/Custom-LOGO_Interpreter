#include "Expression.h"

Expression::Expression() : Node(NodeType::Expression) {};

const void Expression::addExpressionTerm(std::shared_ptr<TermExpression> term)
{
	expressionTerms.push_back(term);
}

const void Expression::addAddOperartor(const TokenType& addOperator)
{
	addOperators.push_back(addOperator);
}

const void Expression::addNextExpressionTerm(std::shared_ptr<TermExpression> term, const TokenType& addOperator)
{
	expressionTerms.push_back(term);
	addOperators.push_back(addOperator);
}

const std::size_t Expression::getTermsSize() const
{
	return expressionTerms.size();
}

std::shared_ptr<TermExpression> Expression::getExpressionTerm(const int& index) const
{
	return expressionTerms[index];
}

TokenType Expression::getOperator(const int& index) const
{
	return addOperators[index];
}
