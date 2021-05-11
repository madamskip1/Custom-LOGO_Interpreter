#pragma once
#include <vector>
#include "Node.h"
class InstructionsBlock :
    public Node
{
public:
    InstructionsBlock() : Node(NodeType::InstructionsBlock) {};

    const void addInstruction(std::shared_ptr<Node> node);
    std::size_t getInstructionsSize() const;
    std::shared_ptr<Node> getInstruction(const int& index) const;

private:
    std::vector<std::shared_ptr<Node>> instructions;
};

