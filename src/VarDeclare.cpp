#include "VarDeclare.h"

VarDeclare::VarDeclare() : Node(NodeType::VarDeclare)
{
}

const void VarDeclare::setType(const TokenType& type)
{
    this->type = type;
}

const void VarDeclare::setIdentifier(std::string id)
{
    identifier = id;
}

TokenType VarDeclare::getType() const
{
    return type;
}

std::string VarDeclare::getIdentifier() const
{
    return identifier;
}

const void VarDeclare::setClassAssignment(std::unique_ptr<ClassAssignment> classAssign)
{
    classAssignment = std::move(classAssign);
}

ClassAssignment* VarDeclare::getClassAssignment() const
{
    return classAssignment.get();
}

const void VarDeclare::setAssignment(std::unique_ptr<AssignmentStatement> assign)
{
    assignment = std::move(assign);
}

AssignmentStatement* VarDeclare::getAssignment() const
{
    return assignment.get();
}
