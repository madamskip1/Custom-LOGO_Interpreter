#include "RepeatTimeStatement.h"
#include <optional>
#include <thread>
#include <chrono>

#include "Context.h"
#include <QDebug>

void runThread(std::unique_ptr<Context> context, std::unique_ptr<AST::InstructionsBlock> block, int period, int howManyTime) {

    auto func = [](std::unique_ptr<Context> insideContext, std::unique_ptr<AST::InstructionsBlock> insideBlock, int insidePeriod, int insideHowManyTime) {
        std::this_thread::sleep_for(std::chrono::milliseconds(insidePeriod));
        for (int i = 0; i < insideHowManyTime; i++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(insidePeriod));
            insideBlock->execute(insideContext.get());

            insideContext->getDrawingBoardPtr()->updateBoard();
            insideContext->getTurtleBoardPtr()->updateBoard();
        }
    };

    std::thread thread(func, std::move(context), std::move(block), period, howManyTime);
    thread.detach();
}

AST::RepeatTimeStatement::RepeatTimeStatement(std::unique_ptr<AST::Expression> period, std::unique_ptr<AST::InstructionsBlock> block) : AST::Node(AST::NodeType::RepeatTimeStatement)
{
	periodExpression = std::move(period);
	instructionsBlock = std::move(block);
	howManyTimeExpression = nullptr;
}

AST::RepeatTimeStatement::RepeatTimeStatement(std::unique_ptr<AST::Expression> period, std::unique_ptr<AST::Expression> howManyTime, std::unique_ptr<AST::InstructionsBlock> block) : AST::Node(AST::NodeType::RepeatTimeStatement)
{
	periodExpression = std::move(period);
	howManyTimeExpression = std::move(howManyTime);
    instructionsBlock = std::move(block);
}

void AST::RepeatTimeStatement::execute(Context *context)
{
    periodExpression->evaluate(context);
    int period = std::get<int>(context->evaluateValue);
    // max 10 iterations
    int howManyTime = 10;

    if (howManyTimeExpression != nullptr)
    {
        howManyTimeExpression->evaluate(context);
        howManyTime = std::get<int>(context->evaluateValue);
    }

    std::unique_ptr<Context> newContext = std::make_unique<Context>();
    newContext->setDrawingBoard(context->getDrawingBoardPtr());
    newContext->setTurtleBoard(context->getTurtleBoardPtr());
    std::vector<std::shared_ptr<Variable>> currentVariables = context->getAllCurrentVariables();
    for (auto var : currentVariables)
    {
        newContext->addVariable(var);
    }
    runThread(std::move(newContext), std::move(instructionsBlock), period, howManyTime);
}


AST::Expression* AST::RepeatTimeStatement::getPeriod() const
{
	return periodExpression.get();
}

AST::Expression* AST::RepeatTimeStatement::getHowManyTime() const
{
	return howManyTimeExpression.get();
}

AST::InstructionsBlock* AST::RepeatTimeStatement::getInstructuionsBlock() const
{
	return instructionsBlock.get();
}
