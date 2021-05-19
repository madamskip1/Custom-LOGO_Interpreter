#pragma once
#include <vector>
#include <string>
#include "Node.h"
#include "Assignable.h"

class AssignmentStatement :
    public Node
{
public:
    AssignmentStatement();

    const void addIdentifier(std::string id);
    const void addIdentifiers(const std::vector<std::string>& ids);
    std::string getIdentifier(int index = 0) const;

    const void setAssign(std::unique_ptr<Assignable> assign);
    Assignable* getAssign() const;

private:
    std::vector<std::string> identifiers;
    std::unique_ptr<Assignable> assign;
};

