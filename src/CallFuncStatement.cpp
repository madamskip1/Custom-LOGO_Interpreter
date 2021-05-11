#include "CallFuncStatement.h"

const void CallFuncStatement::addArgument(std::shared_ptr<Expression> arg)
{
    arguments.push_back(arg);
}

const void CallFuncStatement::addIdentifier(const std::string id)
{
    identifiers.push_back(id);
}

const void CallFuncStatement::addIdentifiers(const std::vector<std::string>& ids)
{
    for (auto const& id : ids)
        identifiers.push_back(id);
}

std::size_t CallFuncStatement::getArgumentsSize() const
{
    return arguments.size();
}

std::shared_ptr<Expression> CallFuncStatement::getArgument(int index) const
{
    return arguments[index];
}

std::size_t CallFuncStatement::getIdentifiersSize() const
{
    return identifiers.size();
}

std::string CallFuncStatement::getIdentifier(int index) const
{
    return identifiers[index];
}
