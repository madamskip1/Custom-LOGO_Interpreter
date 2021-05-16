#pragma once
#include <memory>
#include "Node.h"
#include "InstructionsBlock.h"
#include "Expression.h"

class RepeatStatement :
    public Node
{
public:
    RepeatStatement();

    const void setHowManyTime(std::unique_ptr<Expression> howMany);
    Expression* getHowManyTime() const;

    const void setInstructionsBlock(std::unique_ptr<InstructionsBlock> block);
    InstructionsBlock* getInstructuionsBlock() const;

private:
    std::shared_ptr<Expression> howManyTime = nullptr;
    std::unique_ptr<InstructionsBlock> instructionsBlock = nullptr;
};

