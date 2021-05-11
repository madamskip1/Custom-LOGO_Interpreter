#include "InstructionsBlock.h"

const void InstructionsBlock::addInstruction(std::shared_ptr<Node> node)
{
    instructions.push_back(node);
}

std::size_t InstructionsBlock::getInstructionsSize() const
{
    return instructions.size();
}

std::shared_ptr<Node> InstructionsBlock::getInstruction(const int& index) const
{
    return instructions[index];
}
