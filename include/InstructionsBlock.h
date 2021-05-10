#pragma once
#include "Node.h"
class InstructionsBlock :
    public Node
{
public:
    InstructionsBlock() : Node(NodeType::InstructionsBlock) {};
};

