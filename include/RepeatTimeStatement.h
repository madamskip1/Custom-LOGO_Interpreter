#pragma once
#include "Node.h"
class RepeatTimeStatement :
    public Node
{
public:
    RepeatTimeStatement() : Node(NodeType::RepeatTimeStatement), howManyTime(0), period(0) {};

private:
    int howManyTime;
    int period;
    // std::shared_ptr<BlockInstruction> blockNode;
};

