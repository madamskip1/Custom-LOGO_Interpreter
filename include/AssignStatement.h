#pragma once
#include <vector>
#include <string>
#include "Node.h"
#include "Expression.h"


class AssignStatement :
    public Node
{
public:
    AssignStatement() : Node(NodeType::AssignStatement) {};

    const void addIdentifier(std::string id);
    const void addIdentifiers(const std::vector<std::string>& ids);
    std::string getIdentifier(int index = 0) const;

    const void setExpression(std::shared_ptr<Expression> expr);
    std::shared_ptr<Expression> getExpression() const;

private:
    std::vector<std::string> identifiers;
    std::shared_ptr<Expression> expression = nullptr;
};

