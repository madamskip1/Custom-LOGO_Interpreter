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
    if (identifiers[0] == "size")
    {
        context->evaluateValues.push_back(size);
        return;
    }
    else if(identifiers[0] == "enabled")
    {
        context->evaluateValues.push_back(enabled);
    }
    else if (identifiers[0] == "color")
    {
        if (identifiers.size() == 1 || identifiers[1] == "hex")
        {
            context->evaluateValues.push_back(color);
            return;
        }
        if (identifiers[1] == "R")
        {
            context->evaluateValues.push_back(getColorR());
            return;
        }
        else if (identifiers[1] == "G")
        {
            context->evaluateValues.push_back(getColorG());
            return;
        }
        if (identifiers[1] == "R")
        {
           context->evaluateValues.push_back(getColorB());
            return;
        }
    }
}

void Brush::setSomeVal(std::vector<std::string> identifiers, Context *context)
{
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
