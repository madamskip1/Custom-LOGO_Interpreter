#include "AssignStatement.h"

AssignStatement::AssignStatement() : Node(NodeType::AssignStatement) {};

const void AssignStatement::addIdentifier(std::string id)
{
    identifiers.push_back(id);
}

const void AssignStatement::addIdentifiers(const std::vector<std::string>& ids)
{
    for (auto const& id : ids)
        identifiers.push_back(id);
}

std::string AssignStatement::getIdentifier(int index) const
{
    return identifiers[index];
}

const void AssignStatement::setExpression(std::shared_ptr<Expression> expr)
{
    expression = expr;
}

std::shared_ptr<Expression> AssignStatement::getExpression() const
{
    return expression;
}

//const void AssignStatement::setCondition(std::shared_ptr<Condition> cond)
//{
//    condition = cond;
//}
//
//std::shared_ptr<Condition> AssignStatement::getCondition() const
//{
//    return condition;
//}
