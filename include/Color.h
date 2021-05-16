#pragma once
#include <string>
#include "Assignable.h"

class Color :
    public Assignable
{
public:
    Color(std::string color);

    const void setColor(std::string color);
    std::string getColor() const;

private:
    std::string colorHex;
};

