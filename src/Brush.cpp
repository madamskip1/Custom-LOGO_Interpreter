#include "Brush.h"
#include "Context.h"

const int Brush::DEFAULT_SIZE = 2;
const std::string Brush::DEFAULT_COLOR = "#3498DB";


Brush::Brush() : size(DEFAULT_SIZE), color(DEFAULT_COLOR), enabled(true)
{
}

int Brush::getSize() const
{
    return size;
}

std::string Brush::getColor() const
{
    return color;
}

bool Brush::getEnabled() const
{
    return enabled;
}

void Brush::getSomeVal(std::vector<std::string> identifiers, Context *context)
{
    if (identifiers.size() == 0)
    {
        context->evaluateValue = this;
        return;
    }
    if (identifiers[0] == "size")
    {
        context->evaluateValue = size;
        return;
    }
    if(identifiers[0] == "enabled")
    {
        context->evaluateValue = enabled;
        return;
    }
    if (identifiers[0] == "color")
    {
        if (identifiers.size() == 1 || identifiers[1] == "hex")
        {
            context->evaluateValue = color;
            return;
        }
        if (identifiers[1] == "R")
        {
            context->evaluateValue = getColorR();
            return;
        }
        else if (identifiers[1] == "G")
        {
            context->evaluateValue =getColorG();
            return;
        }
        if (identifiers[1] == "R")
        {
           context->evaluateValue = getColorB();
            return;
        }
    }
}

void Brush::setSomeVal(std::vector<std::string> identifiers, Context *context)
{
    if (identifiers.size() == 0)
    {
        if (context->evaluateValue.index() != 4)
            throw "wrong Brush assignment";

        Variable* pointVar = std::get<Variable*>(context->evaluateValue);

        Brush* brush = static_cast<Brush*>(pointVar);
        size = brush->getSize();
        enabled = brush->getEnabled();
        color = brush->getColor();
        return;
    }
    if (identifiers[0] == "color")
    {
        color = std::get<std::string>(context->setVariant);
        context->setVariant = std::monostate{};
        return;
    }
    else if (identifiers[0] == "size")
    {
        size = std::get<int>(context->setVariant);
        context->setVariant = std::monostate{};
        return;
    }
    else if (identifiers[0] == "enabled")
    {
        enabled = std::get<bool>(context->setVariant);
        context->setVariant = std::monostate{};
        return;
    }
}

std::string Brush::getColorR() const
{
    return color.substr(1, 2);
}

std::string Brush::getColorG() const
{
    return color.substr(3, 2);
}

std::string Brush::getColorB() const
{
    return color.substr(5, 2);
}
