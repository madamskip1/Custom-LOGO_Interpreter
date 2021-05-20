#include "AssignmentStatement.h"

AssignmentStatement::AssignmentStatement(std::vector<std::string> ids, std::unique_ptr<Assignable> assignable) : Node(NodeType::AssignmentStatement)
{
    identifiers = ids;
    assign = std::move(assignable);
}

std::string AssignmentStatement::getIdentifier(int index) const
{
    return identifiers[index];
}


Assignable* AssignmentStatement::getAssign() const
{
    return assign.get();
}
