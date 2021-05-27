#pragma once
#include <string>
#include <vector>
#include "Expression.h"

namespace AST
{
    class VariableExpression :
        public AST::Expression
    {
    public:
        VariableExpression() = delete;
        VariableExpression(std::vector<std::string> variableIdentifiers);

        virtual int evaluate(Context* context) const override;

        virtual bool isOnlyId() const override;
        virtual std::vector<std::string> getIdentifiers() const override;

        std::string getIdentifier(const int& index = 0) const;

    private:
        std::vector<std::string> identifiers;
    };
}
