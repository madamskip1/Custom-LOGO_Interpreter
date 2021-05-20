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
    VarDeclare() = delete;
    VarDeclare(TokenType type, std::string identifier);
    VarDeclare(TokenType type, std::string identifier, std::unique_ptr<AssignmentStatement> assignStatement);
    VarDeclare(TokenType type, std::string identifier, std::unique_ptr<ClassAssignment> classAssign);

    TokenType getType() const;
    std::string getIdentifier() const;

    ClassAssignment* getClassAssignment() const;
    AssignmentStatement* getAssignment() const;

private:
    TokenType type = TokenType::UNKNOWN;
    std::string identifier;
    std::unique_ptr<ClassAssignment> classAssignment;
    std::unique_ptr<AssignmentStatement> assignment;
};

