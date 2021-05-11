#include "Expression.h"

void Expression::addExpressionTerm(std::shared_ptr<ExpressionTerm> term)
{
	expressionTerms.push_back(term);
}

void Expression::addAddOperartor(TokenType addOperator)
{
	addOperators.push_back(addOperator);
}

void Expression::addNextExpressionTerm(std::shared_ptr<ExpressionTerm> term, TokenType addOperator)
{
	expressionTerms.push_back(term);
	addOperators.push_back(addOperator);
}

std::size_t Expression::getTermsSize()
{
	return expressionTerms.size();
}

std::shared_ptr<ExpressionTerm> Expression::getExpressionTerm(int index)
{
	return expressionTerms[index];
}

TokenType Expression::getOperator(int index)
{
	return addOperators[index];
}
