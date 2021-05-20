#pragma once

#include "Node.h"
#include "InstructionsBlock.h"
#include "Condition.h"

class IfStatement :
    public Node
{
public:
    IfStatement() = delete;
    IfStatement(std::unique_ptr<Node> condition, std::unique_ptr <InstructionsBlock> trueBlock, std::unique_ptr <InstructionsBlock> elseBlock = nullptr);

    const bool hasElseBlock() const;
    InstructionsBlock* getTrueBlockNode() const;
    InstructionsBlock* getElseBlockNode() const;
    Node* getCondition() const;

private:
    std::unique_ptr<Node> conditionNode = nullptr;
    std::unique_ptr<InstructionsBlock> trueBlockNode = nullptr;
    std::unique_ptr<InstructionsBlock> elseBlockNode = nullptr;
};