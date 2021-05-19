#pragma once
#include "Condition.h"
#include "Assignable.h"

class Boolean :
    public Assignable
{
public:
    Boolean(bool val);

    const bool getValue() const;
    bool evaluate() const;
private:
    bool value;
};

