#pragma once
#include <memory>
#include "Node.h"
#include "InstructionsBlock.h"
#include "Expression.h"

class RepeatTimeStatement :
    public Node
{
public:
    RepeatTimeStatement() = delete;
    RepeatTimeStatement(std::unique_ptr<Expression> period, std::unique_ptr<InstructionsBlock> block);
    RepeatTimeStatement(std::unique_ptr<Expression> period, std::unique_ptr<Expression> howManyTime, std::unique_ptr<InstructionsBlock> block);

    Expression* getPeriod() const;
    Expression* getHowManyTime() const;
    InstructionsBlock* getInstructuionsBlock() const;

private:
    std::unique_ptr<Expression> periodExpression = nullptr;
    std::unique_ptr<Expression> howManyTimeExpression = nullptr;
    std::unique_ptr<InstructionsBlock> instructionsBlock = nullptr;
};

