#include "VarDeclare.h"

VarDeclare::VarDeclare(TokenType type, std::string identifier) : Node(NodeType::VarDeclare)
{
    this->type = type;
    this->identifier = identifier;
    classAssignment = nullptr;
    assignment = nullptr;
}

VarDeclare::VarDeclare(TokenType type, std::string identifier, std::unique_ptr<AssignmentStatement> assignStatement) : Node(NodeType::VarDeclare)
{
    this->type = type;
    this->identifier = identifier;
    assignment = std::move(assignStatement);
    classAssignment = nullptr;
}

VarDeclare::VarDeclare(TokenType type, std::string identifier, std::unique_ptr<ClassAssignment> classAssign) : Node(NodeType::VarDeclare)
{
    this->type = type;
    this->identifier = identifier;
    classAssignment = std::move(classAssign);
    assignment = nullptr;
}

TokenType VarDeclare::getType() const
{
    return type;
}

std::string VarDeclare::getIdentifier() const
{
    return identifier;
}

ClassAssignment* VarDeclare::getClassAssignment() const
{
    return classAssignment.get();
}

AssignmentStatement* VarDeclare::getAssignment() const
{
    return assignment.get();
}
