#pragma once
#include "Node.h"
#include "Expression.h"
class Expression;

class ExpressionFactor :
    public Node
{
public:
    ExpressionFactor() : Node(NodeType::ExpressionFactor) {};
    const void setNegativeOp(const bool & negative);
    const void setExpression(std::shared_ptr<Expression> expr);
    const void setIntVal(const int& val);
private:
    bool negativeOp = false;
    int intVal = 0;
    std::shared_ptr<Expression> expression = nullptr;
    // Function Call
    // id (var)
};

