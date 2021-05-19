#pragma once

#include "Node.h"
#include "InstructionsBlock.h"
#include "Condition.h"

class IfStatement :
    public Node
{
public:
    IfStatement();

    const void setTrueBlockNode(std::unique_ptr<InstructionsBlock> block);
    const void setElseBlockNode(std::unique_ptr<InstructionsBlock> block);
    const void setCondition(std::unique_ptr<Node> cond);

    const bool hasElseBlock() const;
    InstructionsBlock* getTrueBlockNode() const;
    InstructionsBlock* getElseBlockNode() const;
    Node* getCondition() const;

private:
    std::unique_ptr<Node> condition = nullptr;
    std::unique_ptr<InstructionsBlock> trueBlockNode = nullptr;
    std::unique_ptr<InstructionsBlock> elseBlockNode = nullptr;
};