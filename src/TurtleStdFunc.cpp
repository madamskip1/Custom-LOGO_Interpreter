#include "TurtleStdFunc.h"
#include "Context.h"

void clean(Context* context)
{
    context->getDrawingBoardPtr()->cleanBoard();
}

void allToStart(Context* context)
{
    context->getTurtleBoardPtr()->setAllToStart();
}

