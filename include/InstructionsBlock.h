#pragma once
#include <vector>
#include <memory>
#include "Node.h"

class InstructionsBlock :
    public Node
{
public:
    InstructionsBlock();

    const void addInstruction(std::shared_ptr<Node> node);
    const std::size_t getInstructionsSize() const;
    std::shared_ptr<Node> getInstruction(const int& index) const;

private:
    std::vector<std::shared_ptr<Node>> instructions;
};

