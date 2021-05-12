#pragma once
#include <string>
#include "Node.h"
#include "TokenType.h"
#include "AssignStatement.h"
#include "AssignClassStatement.h"

class DeclareVarStatement :
    public Node
{
public:
    DeclareVarStatement() : Node(NodeType::DeclareVarStatement) { };

    const void setType(TokenType type);
    const void setIdentifier(std::string id);
    const void setAssignStatement(std::shared_ptr<AssignStatement> assign);
    const void setAssignClassStatement(std::shared_ptr<AssignClassStatement> assignClass);
    const void setColorVal(std::string color);

    TokenType getType() const;
    std::string getIdentifier() const;
    std::shared_ptr<AssignStatement> getAssignStatemnt() const;
    std::shared_ptr<AssignClassStatement> getAssignClassStatement() const;
    std::string getColorVal() const;

private:
    TokenType type = TokenType::UNKNOWN;
    std::string identifier;
    std::shared_ptr<AssignStatement> assignStatement = nullptr;
    std::shared_ptr<AssignClassStatement> assignClassStatement = nullptr;
    std::string colorVal;
};

