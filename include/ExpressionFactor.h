#pragma once
#include <vector>
#include <string>
#include "Node.h"
#include "Expression.h"
#include "CallFuncStatement.h"

class Expression;
class CallFuncStatement;

class ExpressionFactor :
    public Node
{
public:
    ExpressionFactor() : Node(NodeType::ExpressionFactor) {};
    const void setNegativeOp(const bool & negative);
    const void setExpression(std::shared_ptr<Expression> expr);
    const void setIntVal(const int& val);
    const void setCallFunc(std::shared_ptr<CallFuncStatement> func);
    const void setVariable(std::vector<std::string> varName);

    int getIntVal() const;
    bool getNegativeOp() const;
    std::shared_ptr<Expression> getExpression() const;
    std::shared_ptr<CallFuncStatement> getCallFunc() const;
    std::vector<std::string> getVarName() const;

private:
    bool negativeOp = false;
    int intVal = 0;
    std::shared_ptr<Expression> expression = nullptr;
    std::shared_ptr<CallFuncStatement> callFunc = nullptr;
    std::vector<std::string> variable;
};

