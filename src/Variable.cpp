#include "Variable.h"
#include "Context.h"

void Variable::getSomeVal(std::vector<std::string>, Context* context)
{
    context->evaluateValue = value;
}

void Variable::setSomeVal(std::vector<std::string>, Context *context)
{
    value = context->evaluateValue;
}
