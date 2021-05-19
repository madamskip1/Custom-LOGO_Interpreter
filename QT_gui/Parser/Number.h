#pragma once
#include "Node.h"
#include "Expression.h"

class Number :
    public Expression
{
public:
    Number(int value);

    int evaluate() const;

    const int getValue() const;
private:
    int value;
};

