#pragma once
#include <vector>
#include <string>
#include "Node.h"
#include "Expression.h"

class Expression;

class CallFuncStatement :
    public Node
{
public:
    CallFuncStatement() : Node(NodeType::CallFuncStatement) {};
    const void addArgument(std::shared_ptr<Expression> arg);
    const void addIdentifier(const std::string id);

    int getArgumentsSize() const;
    std::shared_ptr<Expression> getArgument(int index) const;
    int getIdentifiersSize() const;
    std::string getIdentifier(int index) const;

private:
    std::vector<std::shared_ptr<Expression>> arguments;
    std::vector<std::string> identifiers;
};

