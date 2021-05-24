#include "AssignmentStatement.h"

AST::AssignmentStatement::AssignmentStatement(std::vector<std::string> ids, std::unique_ptr<AST::Assignable> assignable) : AST::Node(AST::NodeType::AssignmentStatement)
{
    identifiers = ids;
    assign = std::move(assignable);
}

std::string AST::AssignmentStatement::getIdentifier(int index) const
{
    return identifiers[index];
}


AST::Assignable* AST::AssignmentStatement::getAssign() const
{
    return assign.get();
}
