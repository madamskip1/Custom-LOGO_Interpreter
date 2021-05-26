#include "Variable.h"
#include "Context.h"

void Variable::getSomeVal(std::vector<std::string>, Context* context)
{
    context->evaluateValues.push_back(value);
}

void Variable::setSomeVal(std::vector<std::string>, Context *context)
{
    value = context->evaluateValues[0];
}
