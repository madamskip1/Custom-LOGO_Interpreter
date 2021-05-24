#include "CallFuncStatement.h"

AST::CallFuncStatement::CallFuncStatement()
{
	nodeType = AST::NodeType::CallFuncStatement;
}

const void AST::CallFuncStatement::addArgument(std::unique_ptr<AST::Expression> arg)
{
    arguments.push_back(std::move(arg));
}

const void AST::CallFuncStatement::addIdentifier(const std::string id)
{
    identifiers.push_back(id);
}

const void AST::CallFuncStatement::addIdentifiers(const std::vector<std::string>& ids)
{
    for (auto const& id : ids)
        identifiers.push_back(id);
}

const std::size_t AST::CallFuncStatement::getArgumentsSize() const
{
    return arguments.size();
}

AST::Expression* AST::CallFuncStatement::getArgument(int index) const
{
    return arguments[index].get();
}

const std::size_t AST::CallFuncStatement::getIdentifiersSize() const
{
    return identifiers.size();
}

std::string AST::CallFuncStatement::getIdentifier(int index) const
{
    return identifiers[index];
}
