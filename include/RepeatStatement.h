#pragma once
#include "Node.h"
#include "Expression.h"
#include "InstructionsBlock.h"

class RepeatStatement :
    public Node
{
public:
    RepeatStatement() : Node(NodeType::RepeatStatement) {};

    void setHowManyTime(std::shared_ptr<Expression> howMany);
    std::shared_ptr<Expression> getHowManyTime();

    const void setInstructionsBlock(std::shared_ptr<InstructionsBlock> block);
    std::shared_ptr<InstructionsBlock> getInstructuionsBlock() const;


private:
    std::shared_ptr<Expression> howManyTime = nullptr;
    std::shared_ptr<InstructionsBlock> instructionsBlock = nullptr;
};

