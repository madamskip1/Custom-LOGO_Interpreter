#pragma once
#include <string>
#include "Node.h"
#include "TokenType.h"
#include "ClassAssignment.h"
#include "AssignmentStatement.h"

class VarDeclare :
    public Node
{
public:
    VarDeclare();

    const void setType(const TokenType& type);
    const void setIdentifier(std::string id);

    TokenType getType() const;
    std::string getIdentifier() const;

    const void setClassAssignment(std::unique_ptr<ClassAssignment> classAssign);
    ClassAssignment* getClassAssignment() const;

    const void setAssignment(std::unique_ptr<AssignmentStatement> assign);
    AssignmentStatement* getAssignment() const;

private:
    TokenType type = TokenType::UNKNOWN;
    std::string identifier;
    std::unique_ptr<ClassAssignment> classAssignment;
    std::unique_ptr<AssignmentStatement> assignment;
};

