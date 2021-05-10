#pragma once
#include "Node.h"
#include "Expression.h"
#include "InstructionsBlock.h"

class RepeatTimeStatement :
    public Node
{
public:
    RepeatTimeStatement() : Node(NodeType::RepeatTimeStatement) {};
    void setPeriod(std::shared_ptr<Expression> time);
    void setHowManyTime(std::shared_ptr<Expression> howMany);

    std::shared_ptr<Expression> getPeriod() const;
    std::shared_ptr<Expression> getHowManyTime() const;

    const void setInstructionsBlock(std::shared_ptr<InstructionsBlock> block);
    std::shared_ptr<InstructionsBlock> getInstructuionsBlock() const;

private:
    std::shared_ptr<Expression> howManyTime = nullptr;
    std::shared_ptr<Expression> period = nullptr;
    std::shared_ptr<InstructionsBlock> instructionsBlock = nullptr;
};

