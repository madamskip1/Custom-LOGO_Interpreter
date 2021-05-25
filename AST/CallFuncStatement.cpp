#include "CallFuncStatement.h"
#include "../include/Context.h"

AST::CallFuncStatement::CallFuncStatement()
{
	nodeType = AST::NodeType::CallFuncStatement;
}

void AST::CallFuncStatement::execute(Context* context)
{
    std::size_t argumentsSize = arguments.size();

    if (identifiers.size() == 1) 
    {
        if (context->hasFunction(identifiers[0]))
        {
            DefFuncStatement* defFunc = context->getDefFunction(identifiers[0]);

            if (getArgumentsSize() != defFunc->getParametersSize())
                throw "wrong number of arguments";

            Context* newContext = new Context();
            // Przes³aæ w nim parametry

            defFunc->executeFunction(newContext);
        }
    }
    else if (identifiers.size() > 1)
    {
        Variable* firstIdVar = context->getVariable(identifiers[0]);
        if (firstIdVar->type == TokenType::Turtle)
        {
            Turtle* turtle = static_cast<Turtle*>(firstIdVar);
            Context* newContext = new Context();
            for (auto& arg : arguments)
            {
                newContext->args.push_back(arg.get());
            }
            turtle->callFunction(std::vector<std::string>(identifiers.begin() +1, identifiers.end()), newContext);
        }
    }
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
