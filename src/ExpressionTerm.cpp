#include "ExpressionTerm.h"

void ExpressionTerm::addExpressionFactor(std::shared_ptr<ExpressionFactor> factor)
{
	expressionFactors.push_back(factor);
}

void ExpressionTerm::addMutliOperartor(TokenType multiOperator)
{
	multiOperators.push_back(multiOperator);
}
