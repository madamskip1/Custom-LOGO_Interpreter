#pragma once
#include <memory>
#include "Node.h"
#include "InstructionsBlock.h"
#include "Condition.h"

class IfStatement :
    public Node
{
public:
    IfStatement();

    const void setTrueBlockNode(std::shared_ptr<InstructionsBlock> block);
    const void setElseBlockNode(std::shared_ptr<InstructionsBlock> block);
    const void setCondition(std::shared_ptr<Condition> cond);

    const bool hasElseBlock() const;
    std::shared_ptr<InstructionsBlock> getTrueBlockNode() const;
    std::shared_ptr<InstructionsBlock> getElseBlockNode() const;
    std::shared_ptr<Condition> getCondition() const;

private:
    std::shared_ptr<Condition> condition = nullptr;
    std::shared_ptr<InstructionsBlock> trueBlockNode = nullptr;
    std::shared_ptr<InstructionsBlock> elseBlockNode = nullptr;
};

