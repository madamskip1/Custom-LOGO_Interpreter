#pragma once
#include <string>
#include "Node.h"
#include "TokenType.h"
#include "AssignStatement.h"

class DeclareVarStatement :
    public Node
{
public:
    DeclareVarStatement() : Node(NodeType::DeclareVarStatement) { };

    const void setType(TokenType type);
    const void setIdentifier(std::string id);
    const void setAssignStatement(std::shared_ptr<AssignStatement> assign);

    TokenType getType() const;
    std::string getIdentifier() const;
    std::shared_ptr<AssignStatement> getAssignStatemnt() const;

private:
    TokenType type = TokenType::UNKNOWN;
    std::string identifier;
    std::shared_ptr<AssignStatement> assignStatement = nullptr;
};

