#include "CallFuncStatement.h"

CallFuncStatement::CallFuncStatement()
{
	nodeType = NodeType::CallFuncStatement;
}

const void CallFuncStatement::addArgument(std::unique_ptr<Expression> arg)
{
    arguments.push_back(std::move(arg));
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

const std::size_t CallFuncStatement::getArgumentsSize() const
{
    return arguments.size();
}

Expression* CallFuncStatement::getArgument(int index) const
{
    return arguments[index].get();
}

const std::size_t CallFuncStatement::getIdentifiersSize() const
{
    return identifiers.size();
}

std::string CallFuncStatement::getIdentifier(int index) const
{
    return identifiers[index];
}
