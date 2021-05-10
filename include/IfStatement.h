#pragma once
#include <memory>
#include "Node.h"
#include "InstructionsBlock.h"

class IfStatement :
    public Node
{
public:
    IfStatement() : Node(NodeType::IfStatement) {};

    void setTrueBlockNode(std::shared_ptr<InstructionsBlock> block);
    void setElseBlockNode(std::shared_ptr<InstructionsBlock> block);

    std::shared_ptr<InstructionsBlock> getTrueBlockNode();
    std::shared_ptr<InstructionsBlock> getElseBlockNode();

private:
    //std::shared_ptr<Condition> conditionNode;
    std::shared_ptr<InstructionsBlock> trueBlockNode;
    std::shared_ptr<InstructionsBlock> elseBlockNode;
};

