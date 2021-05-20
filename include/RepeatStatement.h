#pragma once
#include <memory>
#include "Node.h"
#include "InstructionsBlock.h"
#include "Expression.h"

class RepeatStatement :
    public Node
{
public:
    RepeatStatement() = delete;
    RepeatStatement(std::unique_ptr<Expression> howManyTime, std::unique_ptr<InstructionsBlock> block);

    Expression* getHowManyTime() const;
    InstructionsBlock* getInstructuionsBlock() const;

private:
    std::shared_ptr<Expression> howManyTimeExpression = nullptr;
    std::unique_ptr<InstructionsBlock> instructionsBlock = nullptr;
};

