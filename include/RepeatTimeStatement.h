#pragma once
#include <memory>
#include "Node.h"
#include "InstructionsBlock.h"
#include "Expression.h"

class RepeatTimeStatement :
    public Node
{
public:
    RepeatTimeStatement();

    const void setPeriod(std::unique_ptr<Expression> time);
    const void setHowManyTime(std::unique_ptr<Expression> howMany);

    Expression* getPeriod() const;
    Expression* getHowManyTime() const;

    const void setInstructionsBlock(std::unique_ptr<InstructionsBlock> block);
    InstructionsBlock* getInstructuionsBlock() const;

private:
    std::unique_ptr<Expression> period = nullptr;
    std::unique_ptr<Expression> howManyTime = nullptr;
    std::unique_ptr<InstructionsBlock> instructionsBlock = nullptr;
};

