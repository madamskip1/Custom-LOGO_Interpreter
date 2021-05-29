#include "CallFuncStatement.h"
#include "../include/Context.h"
#include "Logger.h"
#include <stdexcept>

AST::CallFuncStatement::CallFuncStatement()
{
	nodeType = AST::NodeType::CallFuncStatement;
}

void AST::CallFuncStatement::execute(Context* context)
{
    if (identifiers.size() == 1)
    {
        if (executeStdLibFunc(context))
            return;

        if (executeDefinedFunc(context))
            return;

        Logger::addError(LogType::FuncNotDefined, token);
        throw std::runtime_error("Function not defined");
    }
    else if (identifiers.size() > 1)
    {
        executeTurtleFunc(context);
    }
}

void AST::CallFuncStatement::evaluate(Context *context)
{
    execute(context);
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

bool AST::CallFuncStatement::executeStdLibFunc(Context *context)
{
    if (context->hasStdLibFunction(identifiers[0]))
    {
        std::function<void(Context*)> func = context->getStdLibFunction(identifiers[0]);
        func(context);
        return true;
    }

    return false;
}

bool AST::CallFuncStatement::executeDefinedFunc(Context *context)
{
    if (context->hasFunction(identifiers[0]))
    {
        DefFuncStatement* defFunc = context->getDefFunction(identifiers[0]);
        std::size_t argumentsSize = arguments.size();

        if (argumentsSize != defFunc->getParametersSize())
        {
            Logger::addError(LogType::NotEqualeNumArgs, token);
            throw std::runtime_error("wrong number of arguments");
        }
        std::vector<std::unique_ptr<Variable>> tempVars;

        Context newContext;
        bool argAdded;

        for (std::size_t i = 0; i < argumentsSize; i++)
        {
            argAdded = false;
            auto& arg = arguments[i];
            Parameter* param = defFunc->getParameter(i);

            if (arg->isOnlyId())
            {
                std::vector<std::string> identifiers;
                identifiers = arg->getIdentifiers();
                if (identifiers.size() == 1)
                {
                    Variable* var = context->getVariable(identifiers[0]);
                    if (var->type != param->getType())
                    {
                        Logger::addError(LogType::ArgTypeNotEqualParameter, token);
                        throw std::runtime_error("args type isn't equal to param type");
                    }

                    newContext.args.emplace(param->getName(), context->getVariable(identifiers[0]));
                    argAdded = true;
                }
            }

            if (!argAdded)
            {
                arg->evaluate(context);

                std::unique_ptr<Variable> tempVar;
                tempVar = std::make_unique<Variable>();
                tempVar->value = context->evaluateValue;
                newContext.args.emplace(param->getName(), tempVar.get());
                tempVars.push_back(std::move(tempVar));
            }
        }

        newContext.setDrawingBoard(context->getDrawingBoardPtr());
        newContext.setTurtleBoard(context->getTurtleBoardPtr());
        defFunc->executeFunction(&newContext);

        context->evaluateValue = newContext.returnVariant;
        return true;
    }

    return false;
}

void AST::CallFuncStatement::executeTurtleFunc(Context *context)
{
    Variable* firstIdVar = context->getVariable(identifiers[0]);
    if (firstIdVar->type == TokenType::Turtle)
    {
        Turtle* turtle = static_cast<Turtle*>(firstIdVar);
        Context newContext;

        std::vector<std::unique_ptr<Variable>> tempVars;

        int argCounter = 0;
        for (const auto& arg : arguments)
        {
            arg->evaluate(context);
            if (context->evaluateValue.index() == 4)
            {
                Variable* evaluatedVariable = std::get<Variable*>(context->evaluateValue);
                std::string argName { "arg" };
                argName.append(std::to_string(argCounter));

                newContext.args.emplace(argName, evaluatedVariable);
            }
            else
            {
                std::unique_ptr<Variable> tempVar;

                tempVar = std::make_unique<Variable>();
                tempVar->value = context->evaluateValue;
                std::string argName { "arg" };
                argName.append(std::to_string(argCounter));

                newContext.args.emplace(argName, tempVar.get());
                tempVars.push_back(std::move(tempVar));
            }

            argCounter++;
        }
        turtle->callFunction(std::vector<std::string>(identifiers.begin() +1, identifiers.end()), &newContext);
    }
}
