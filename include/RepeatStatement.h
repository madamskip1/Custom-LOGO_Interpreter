#pragma once
#include "Node.h"
#include "Expression.h"

class RepeatStatement :
    public Node
{
public:
    RepeatStatement() : Node(NodeType::RepeatStatement), howManyTime(0) {};

    void setHowManyTime(std::shared_ptr<Expression> howMany);
    std::shared_ptr<Expression> getHowManyTime();
private:
    std::shared_ptr<Expression> howManyTime;
   // std::shared_ptr<BlockInstruction> blockNode;
};

