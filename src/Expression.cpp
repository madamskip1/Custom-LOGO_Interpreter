#include "Expression.h"

void Expression::addExpressionTerm(std::shared_ptr<ExpressionTerm> term)
{
	expressionTerms.push_back(term);
}

void Expression::addAddOperartor(TokenType addOperator)
{
	addOperators.push_back(addOperator);
}
