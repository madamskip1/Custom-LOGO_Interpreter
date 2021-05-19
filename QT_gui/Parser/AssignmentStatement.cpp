#include "AssignmentStatement.h"

AssignmentStatement::AssignmentStatement() : Node(NodeType::AssignmentStatement) {};

const void AssignmentStatement::addIdentifier(std::string id)
{
    identifiers.push_back(id);
}

const void AssignmentStatement::addIdentifiers(const std::vector<std::string>& ids)
{
    for (auto const& id : ids)
        identifiers.push_back(id);
}

std::string AssignmentStatement::getIdentifier(int index) const
{
    return identifiers[index];
}

const void AssignmentStatement::setAssign(std::unique_ptr<Assignable> assign)
{
    this->assign = std::move(assign);
}

Assignable* AssignmentStatement::getAssign() const
{
    return assign.get();
}
