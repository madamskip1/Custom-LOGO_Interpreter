#include "ClassAssignment.h"

AST::ClassAssignment::ClassAssignment() :AST::Node(AST::NodeType::AssignClassStatement) {};

const void AST::ClassAssignment::addExpression(std::unique_ptr<AST::Expression> expression)
{
    expressions.push_back(std::move(expression));
}

std::size_t AST::ClassAssignment::getExpressionsSize() const
{
    return expressions.size();
}

AST::Expression* AST::ClassAssignment::getExpression(int index) const
{
    return expressions[index].get();
}


