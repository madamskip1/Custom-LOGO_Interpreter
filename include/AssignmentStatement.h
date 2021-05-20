#pragma once
#include <vector>
#include <string>
#include "Node.h"
#include "Assignable.h"

class AssignmentStatement :
    public Node
{
public:
    AssignmentStatement() = delete;
    AssignmentStatement(std::vector<std::string> ids, std::unique_ptr<Assignable> assignable);

    std::string getIdentifier(int index = 0) const;

    Assignable* getAssign() const;

private:
    std::vector<std::string> identifiers;
    std::unique_ptr<Assignable> assign;
};

