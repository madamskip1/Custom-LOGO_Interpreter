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

            std::variant<std::monostate, int, std::string, bool, Variable*> temp;
            std::vector<std::unique_ptr<Variable>> tempVars;
            std::unique_ptr<Variable> tempVar;
            std::vector<std::string> identifiers;

            Context newContext;
            bool argAdded;
            for (std::size_t i = 0; i < argumentsSize; i++)
            {
                argAdded = false;
                auto& arg = arguments[i];
                Parameter* param = defFunc->getParameter(i);

                if (arg->isOnlyId())
                {
                    identifiers = arg->getIdentifiers();
                    if (identifiers.size() == 1)
                    {
                        Variable* var = context->getVariable(identifiers[0]);
                        if (var->type != param->getType())
                        {
                           throw "args type isn't equal to param type";
                        }
                        newContext.args.emplace(param->getName(), context->getVariable(identifiers[0]));
                        argAdded = true;
                    }
                }

                if (!argAdded)
                {
                    arg->evaluate(context);

                    tempVar = std::make_unique<Variable>();
                    tempVar->value = context->evaluateValue;
                    newContext.args.emplace(param->getName(), tempVar.get());
                    tempVars.push_back(std::move(tempVar));
                }
            }

            // Przes³aæ w nim parametry
            newContext.setDrawingBoard(context->getDrawingBoardPtr());
            newContext.setTurtleBoard(context->getTurtleBoardPtr());
            defFunc->executeFunction(&newContext);
        }
        else if (context->hasStdLibFunction(identifiers[0]))
        {
            std::function<void(Context*)> func = context->getStdLibFunction(identifiers[0]);
            func(context);
        }
    }
    else if (identifiers.size() > 1)
    {
        std::vector<std::unique_ptr<Variable>> tempVars;

        Variable* firstIdVar = context->getVariable(identifiers[0]);
        if (firstIdVar->type == TokenType::Turtle)
        {
            Turtle* turtle = static_cast<Turtle*>(firstIdVar);
            Context* newContext = new Context();

            std::vector<std::unique_ptr<Variable>> tempVars;
            std::unique_ptr<Variable> tempVar;

            int i = 0;
            for (auto& arg : arguments)
            {
                arg->evaluate(context);
                if (context->evaluateValue.index() == 4)
                {
                    newContext->args.emplace(std::string("arg").append(std::to_string(i)), std::get<Variable*>(context->evaluateValue));
                }
                else
                {
                    tempVar = std::make_unique<Variable>();
                    tempVar->value = context->evaluateValue;
                    newContext->args.emplace(std::string("arg").append(std::to_string(i)), tempVar.get());
                    tempVars.push_back(std::move(tempVar));
                }

                i++;
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
