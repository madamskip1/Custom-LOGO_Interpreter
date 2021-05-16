#include "Parser.h"
#include "TokenType.h"

#include "InstructionsBlock.h"
#include "IfStatement.h"
#include "RepeatStatement.h"
#include "RepeatTimeStatement.h"
#include "Expression.h"
#include "Number.h"
#include "Condition.h"
#include "Boolean.h"
#include "DefFuncStatement.h"
#include "CallFuncStatement.h"
#include "VarDeclare.h"
#include "Variable.h"
#include "Color.h"


Parser::Parser(Lexer* lex, Logger* logger) : lexer(lex), logger(logger)
{
}

std::unique_ptr<Node> Parser::parse()
{
    return parseProgram();
}

std::unique_ptr<Node> Parser::parseProgram()
{
    std::unique_ptr<ProgramRootNode> rootNode = std::make_unique<ProgramRootNode>();

    bool wasError = false;
    do {
        std::unique_ptr<Node> node = parseInstruction();

        if (node)
        {
            rootNode->addChild(std::move(node));

            if (logger->hasAnyError())
                wasError = true;
        }
        else
        {
            wasError = true;
        }

    } while (!checkCurTokenType(TokenType::EndOfFile) && !wasError);

    if (!checkCurTokenType(TokenType::EndOfFile))
    {
        logger->addNewError(LogType::NotEndOfFile, getToken());
    }
    return rootNode;
}

std::unique_ptr<Node> Parser::parseInstruction()
{
    std::unique_ptr<Node> node;

    node = parseIfStatement();
    if (node)
        return node;

    node = parseRepeatStatement();
    if (node)
        return node;

    node = parseRepeatTimeStatement();
    if (node)
        return node;

    node = parseVarDeclareORDefFuncWithReturStatement();
    if (node)
        return node;

    node = parseDefFuncStatement(TokenType::UNKNOWN);
    if (node)
        return node;

    node = parseAssignOrCallFunctionStatement();
    if (node)
        return node;


    return nullptr;
}

std::unique_ptr<InstructionsBlock> Parser::parseInstructionsBlock()
{
    if (!consumeTokenIfType_Otherwise_AddError(TokenType::CurlyBracketOpen, LogType::MissingCurlyBracketOpen))
        return nullptr;

    std::unique_ptr<InstructionsBlock> instructionsBlock = std::make_unique<InstructionsBlock>();

    while (!checkCurTokenType({ TokenType::CurlyBracketClose, TokenType::EndOfFile }))
    {
        if (getToken().type == TokenType::Return)
        {
            consumeToken();
            std::unique_ptr<ReturnStatement> returnStatement = parseReturnStatement();

            if (!returnStatement)
                return nullptr;

            instructionsBlock->addChild(std::move(returnStatement));
        }
        else
        {
            Token token = getToken();
            std::unique_ptr<Node> instruction;
            instruction = parseInstruction();

            if (!instruction)
                return nullptr;

            if (instruction->getNodeType() == NodeType::DefFuncStatement)
            {
                logger->addNewError(LogType::CantDefFuncInBlock, token);
                return nullptr; 
            }

            instructionsBlock->addChild(std::move(instruction));
        }
    }

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::CurlyBracketClose, LogType::MissingCurlyBracketClose))
        return nullptr;

    return instructionsBlock;
}

std::unique_ptr<Node> Parser::parseIfStatement()
{
    if (!consumeTokenIfType(TokenType::If))
        return nullptr;


    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketOpen, LogType::MissingRoundBracketOpen))
        return nullptr;

    Token token = getToken();
    std::unique_ptr<Node> condition = parseCondition();

    if (!condition)
        return nullptr;

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
        return nullptr;

    std::unique_ptr<InstructionsBlock> trueBlock = parseInstructionsBlock();
    if (!trueBlock)
        return nullptr;

    std::unique_ptr<InstructionsBlock> elseBlock;
    if (consumeTokenIfType(TokenType::Else))
    {
        elseBlock = parseInstructionsBlock();
        if (!trueBlock)
            return elseBlock;
    }

    std::unique_ptr<IfStatement> ifStatement = std::make_unique<IfStatement>();
    ifStatement->setCondition(std::move(condition));
    ifStatement->setTrueBlockNode(std::move(trueBlock));
    
    if (elseBlock)
        ifStatement->setElseBlockNode(std::move(elseBlock));

    return ifStatement;
}

std::unique_ptr<Node> Parser::parseRepeatStatement()
{
    if (!consumeTokenIfType(TokenType::Repeat))
        return nullptr;

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketOpen, LogType::MissingRoundBracketOpen))
        return nullptr;

    std::unique_ptr<Expression> howManyTime = parseExpression();
    if (!howManyTime)
        return nullptr;

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
        return nullptr;

    std::unique_ptr<InstructionsBlock> block = parseInstructionsBlock();
    if (!block)
        return nullptr;

    std::unique_ptr<RepeatStatement> repeatStatement = std::make_unique<RepeatStatement>();
    repeatStatement->setHowManyTime(std::move(howManyTime));
    repeatStatement->setInstructionsBlock(std::move(block));
    return repeatStatement;
}

std::unique_ptr<Node> Parser::parseRepeatTimeStatement()
{
    if (!consumeTokenIfType(TokenType::RepeatTime))
        return nullptr;

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketOpen, LogType::MissingRoundBracketOpen))
        return nullptr;

    std::unique_ptr<Expression> period = parseExpression();
    if (!period)
        return nullptr;

    std::unique_ptr<Expression> howManyTime = nullptr;
    if (consumeTokenIfType(TokenType::Comma))
    {
        howManyTime = parseExpression();
        if (!howManyTime)
            return nullptr;
    }

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
        return nullptr;

    std::unique_ptr<InstructionsBlock> block = parseInstructionsBlock();
    if (!block)
        return nullptr;

    std::unique_ptr<RepeatTimeStatement> repeatTimeStatement = std::make_unique<RepeatTimeStatement>();
    repeatTimeStatement->setPeriod(std::move(period));
    if (howManyTime)
    {
        repeatTimeStatement->setHowManyTime(std::move(howManyTime));
    }
    repeatTimeStatement->setInstructionsBlock(std::move(block));
    return repeatTimeStatement;
}

std::unique_ptr<Node> Parser::parseAssignOrCallFunctionStatement()
{
    std::vector<std::string> identifiers = parseIdentifiers();

    if (identifiers.empty())
        return nullptr;

    std::unique_ptr<Node> assignOrCall = parseCallFunctionStatement(identifiers);
    if (assignOrCall)
    {
        consumeTokenIfType_Otherwise_AddLog(TokenType::Semicolon, LogType::MissingSemicolon);
        return assignOrCall;
    }

    if (consumeTokenIfType(TokenType::AssignOperator))
    {
        assignOrCall = parseAssignment(identifiers);
        if (assignOrCall)
            return assignOrCall;
    }
    
    return nullptr;

}

std::unique_ptr<CallFuncStatement> Parser::parseCallFunctionStatement(std::vector<std::string> identifiers)
{
    if (!consumeTokenIfType(TokenType::RoundBracketOpen))
        return nullptr;
    
    std::unique_ptr<CallFuncStatement> callFunc = std::make_unique<CallFuncStatement>();

    if (!consumeTokenIfType(TokenType::RoundBracketClose))
    {
        do {
            Token token = getToken();
            std::unique_ptr<Expression> arg = parseExpression();
            if (!arg)
            {
                logger->addNewError(LogType::MissingParameter, token);
                return nullptr;
            }

            callFunc->addArgument(std::move(arg));

        } while (consumeTokenIfType(TokenType::Comma));

        if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
            return nullptr;
    }

    callFunc->addIdentifiers(identifiers);

    return callFunc;
}

std::unique_ptr<Node> Parser::parseVarDeclareORDefFuncWithReturStatement()
{
    Token token = getToken();
    
    if (!consumeTokenIfType({ { TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean } }))
        return nullptr;

    std::unique_ptr<Node> decVarOrDefFunc;
    decVarOrDefFunc = parseDefFuncStatement(token.type);

    if (decVarOrDefFunc)
        return decVarOrDefFunc;

    decVarOrDefFunc = parseVarDeclare(token.type);

    return decVarOrDefFunc;
}

std::unique_ptr<VarDeclare> Parser::parseVarDeclare(const TokenType& type)
{
	std::string identifier = getToken().getStringValue();

	if (!consumeTokenIfType_Otherwise_AddError(TokenType::Identifier, LogType::MissingIdentifierOrFunctionKeyword))
		return nullptr;

    std::unique_ptr<ClassAssignment> classAssign;
    std::unique_ptr<AssignmentStatement> assign;

    if (checkCurTokenType(TokenType::AssignOperator))
    {
        consumeToken();
        assign = parseAssignment(std::vector<std::string>());
        if (!assign)
            return nullptr;
    }
    else if (checkTokenType(type, { TokenType::Point, TokenType::Turtle }) && checkCurTokenType(TokenType::RoundBracketOpen))
    {
        classAssign = parseClassAssignment();
        if (!classAssign)
            return nullptr;
    }

    consumeTokenIfType_Otherwise_AddLog(TokenType::Semicolon, LogType::MissingSemicolon);

    std::unique_ptr<VarDeclare> varDeclare = std::make_unique<VarDeclare>();
    varDeclare->setType(type);
    varDeclare->setIdentifier(identifier);

    if (classAssign)
        varDeclare->setClassAssignment(std::move(classAssign));
    else if (assign)
        varDeclare->setAssignment(std::move(assign));

    return varDeclare;
}

std::unique_ptr<Node> Parser::parseDefFuncStatement(const TokenType& returnType)
{
    if (!consumeTokenIfType(TokenType::Function))
        return nullptr;

    std::string name = getToken().getStringValue();

    if (name == "" || !consumeTokenIfType_Otherwise_AddError(TokenType::Identifier, LogType::MissingIdentifier))
        return nullptr;

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketOpen, LogType::MissingRoundBracketOpen))
        return nullptr;

    std::unique_ptr<DefFuncStatement> defFunc = std::make_unique<DefFuncStatement>();
    if (!consumeTokenIfType(TokenType::RoundBracketClose))
    {
        do {
            Token token = getToken();
            std::unique_ptr<Parameter> parameter = parseParameter();

            if (!parameter)
            {
                logger->newLog(LogType::MissingParameter, token);
                return nullptr;
            }

            defFunc->addParameter(std::move(parameter));
        } while (consumeTokenIfType(TokenType::Comma));
        
        if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
            return nullptr;

    }

    std::unique_ptr<InstructionsBlock> instructionsBlock = parseInstructionsBlock();

    if (!instructionsBlock)
        return nullptr;

    defFunc->setInstructionsBlock(std::move(instructionsBlock));
    defFunc->setName(name);
    defFunc->setReturnType(returnType);
    return defFunc;
}

std::unique_ptr<AssignmentStatement> Parser::parseAssignment(std::vector<std::string> identifiers)
{
    std::unique_ptr<Assignable> assignable = parseAssignable();

    if (!assignable)
        return nullptr;

    std::unique_ptr<AssignmentStatement> assign = std::make_unique<AssignmentStatement>();
    assign->addIdentifiers(identifiers);
    assign->setAssign(std::move(assignable));
    return assign;
}

std::unique_ptr<Assignable> Parser::parseAssignable()
{
    Token token = getToken();

    if (checkCurTokenType({ TokenType::True, TokenType::False }))
    {
        std::unique_ptr<Assignable> assignable = std::make_unique<Boolean>(checkCurTokenType(TokenType::True));
        consumeToken();

        return assignable;
    }

    if (checkCurTokenType(TokenType::ColorValue))
    {
        std::unique_ptr<Assignable> assignable = std::make_unique<Color>(getAndConsumeToken().getStringValue());
        
        return assignable;
    }

    std::unique_ptr<Assignable> assignable = parseExpression();
    
    if (assignable)
        return assignable;

    logger->addNewError(LogType::UnknownAssignable, token);
    return nullptr;
}

std::unique_ptr<ClassAssignment> Parser::parseClassAssignment()
{
    if (!consumeTokenIfType(TokenType::RoundBracketOpen))
        return nullptr;
    
    if (checkCurTokenType(TokenType::RoundBracketClose))
    {
        logger->addNewError(LogType::BadExpression, getToken());
        return nullptr;
    }

    std::unique_ptr<ClassAssignment> classAssign = std::make_unique<ClassAssignment>();

    do {
        Token token = getToken();
        std::unique_ptr<Expression> expression = parseExpression();
        
        if (!expression)
        {
            logger->addNewError(LogType::BadExpression, token);
            return nullptr;
        }

        classAssign->addExpression(std::move(expression));

    } while (consumeTokenIfType(TokenType::Comma));

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingCurlyBracketClose))
        return nullptr;

    return classAssign;
}

std::unique_ptr<Parameter> Parser::parseParameter()
{
    TokenType paramType = getToken().type;
    
    if (!consumeTokenIfType({ TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean }))
    {
        logger->newLog(LogType::BadSyntaxParameter, getToken());
        return nullptr;
    }

    std::string identifier = getToken().getStringValue();

    if (identifier == "" || !consumeTokenIfType_Otherwise_AddError(TokenType::Identifier, LogType::MissingIdentifier))
        return nullptr;

    std::unique_ptr<Parameter> parameter = std::make_unique<Parameter>();
    parameter->setName(identifier);
    parameter->setType(paramType);

    return parameter;
}

std::unique_ptr<Expression> Parser::parseExpression()
{
    std::unique_ptr<Expression> termExpression = parseTermExpression();
    
    if (!termExpression)
        return nullptr;

    std::unique_ptr<Expression> expression = std::make_unique<Expression>();
    expression->addChildExpression(std::move(termExpression));
    
    Token token = getToken();

    while (consumeTokenIfType({ TokenType::Plus, TokenType::Minus }))
    {
        termExpression = parseTermExpression();
        if (!termExpression)
            return nullptr;

        expression->addChildExpression(std::move(termExpression));
        expression->addOperator(token.type);
        token = getToken();
    }

    return expression;
}

std::unique_ptr<Expression> Parser::parseTermExpression()
{
    std::unique_ptr<Expression> factorExpression = parseFactorExpression();

    if (!factorExpression)
        return nullptr;

    std::unique_ptr<Expression> termExpression = std::make_unique<Expression>();
    termExpression->addChildExpression(std::move(factorExpression));

    Token token = getToken();

    while (consumeTokenIfType({ TokenType::Divide, TokenType::Multiply }))
    {
        factorExpression = parseFactorExpression();
        if (!factorExpression)
            return nullptr;

        termExpression->addChildExpression(std::move(factorExpression));
        termExpression->addOperator(token.type);
        token = getToken();
    }

    return termExpression;
}

std::unique_ptr<Expression> Parser::parseFactorExpression()
{
    bool negativeOp = consumeTokenIfType(TokenType::Minus);


    if (checkCurTokenType(TokenType::Digit))
    {
        std::unique_ptr<Number> factorExpression = std::make_unique<Number>(getToken().getIntValue());
        factorExpression->setNegativeOp(negativeOp);
        consumeToken();
        return factorExpression;
    }

    if (consumeTokenIfType(TokenType::RoundBracketOpen))
    {
        std::unique_ptr<Expression> factorExpression = parseExpression();
        
        if (!factorExpression)
            return nullptr;

        if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
            return nullptr;

        factorExpression->setNegativeOp(negativeOp);
        return factorExpression;
    }

    if (checkCurTokenType(TokenType::Identifier))
    {
        std::vector<std::string> identifiers = parseIdentifiers();

        if (identifiers.empty())
            return nullptr;

        if (checkCurTokenType(TokenType::RoundBracketOpen))
        {
            std::unique_ptr<CallFuncStatement> callFunc = parseCallFunctionStatement(identifiers);
            
            if (!callFunc)
                return nullptr;

            return callFunc;
        }

        std::unique_ptr<Variable> var = std::make_unique<Variable>();
        var->setIdentfiers(identifiers);
        var->setNegativeOp(negativeOp);

        return var;
    }

    logger->addNewError(LogType::BadExpression, getToken());
    return nullptr;
}

std::unique_ptr<Node> Parser::parseCondition()
{
    std::unique_ptr<Node> andCondition = parseAndCondition();
    if (!andCondition)
        return nullptr;

    std::unique_ptr<Condition> condition = std::make_unique<Condition>();
    condition->setCondition(std::move(andCondition));

    while (consumeTokenIfType(TokenType::Or))
    {
        andCondition = parseAndCondition();
        if (!andCondition)
            return nullptr;

        condition->setRightCondition(std::move(andCondition));
    }

    return condition;
}

std::unique_ptr<Node> Parser::parseAndCondition()
{
    std::unique_ptr<Node> relationCondition = parseRelationCondition();
    if (!relationCondition)
        return nullptr;

    std::unique_ptr<Condition> andCondition = std::make_unique<Condition>();
    andCondition->setCondition(std::move(relationCondition));
    
    while (consumeTokenIfType(TokenType::And))
    {
        relationCondition = parseRelationCondition();
        if (!relationCondition)
            return nullptr;
        andCondition->setRightCondition(std::move(relationCondition));
    }

    return andCondition;
}

std::unique_ptr<Node> Parser::parseRelationCondition()
{
    bool notOperator = consumeTokenIfType(TokenType::NotOperator);

    if (checkCurTokenType({ TokenType::True, TokenType::False }))
    {
        bool boolean = getAndConsumeToken().type == TokenType::True;
        std::unique_ptr<Boolean> booleanWord = std::make_unique<Boolean>(boolean);
        std::unique_ptr<Condition> conditionBoolean = std::make_unique<Condition>();

        conditionBoolean->setCondition(std::move(booleanWord));
        conditionBoolean->setNotOperator(notOperator);

        return conditionBoolean;
    }

    if (consumeTokenIfType(TokenType::RoundBracketOpen))
    {
        std::unique_ptr<Node> condition = parseCondition();
        if (!condition)
            return nullptr;

        if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingCurlyBracketClose))
            return nullptr;

        Condition* tempCon = static_cast<Condition*>(condition.get());
        tempCon->setNotOperator(notOperator);
        return condition;
    }
    
    std::unique_ptr<Node> expression = parseExpression();
    if (!expression)
        return nullptr;


    std::unique_ptr<Condition> relationCondition = std::make_unique<Condition>();
    if (checkCurTokenType({ TokenType::Equal, TokenType::NotEqual, TokenType::Less, TokenType::Greater, TokenType::LessOrEqual, TokenType::GreaterOrEqual }))
    {
        TokenType relOp = getAndConsumeToken().type;
        std::unique_ptr<Node> secondExpression = parseExpression();

        if(!secondExpression)
            return nullptr;

        relationCondition->setRelationOperator(relOp);
        relationCondition->setRightCondition(std::move(secondExpression));
    }

    relationCondition->setCondition(std::move(expression));

    return relationCondition;
}

std::unique_ptr<ReturnStatement> Parser::parseReturnStatement()
{
    std::unique_ptr<Assignable> assignable = parseAssignable();

    if (!assignable)
        return nullptr;

    std::unique_ptr<ReturnStatement> returnStatement = std::make_unique<ReturnStatement>();
    returnStatement->setReturn(std::move(assignable));

    consumeTokenIfType_Otherwise_AddLog(TokenType::Semicolon, LogType::MissingSemicolon);

    return returnStatement;
}

std::vector<std::string> Parser::parseIdentifiers()
{
    if (!checkCurTokenType(TokenType::Identifier))
    {
        return std::vector<std::string>();
    }
        
    std::vector<std::string> identifiers;
    std::string identifier;

    do {
        identifier = getToken().getStringValue();

        if (identifier != "" && consumeTokenIfType(TokenType::Identifier))
        {
            identifiers.push_back(identifier);
        }
        else
        {
            logger->addNewError(LogType::MissingIdentifier, getToken());
            return std::vector<std::string>();
        }

    } while (consumeTokenIfType(TokenType::Dot));

    return identifiers;
}



Token Parser::getToken()
{
    if (token)
        return *token;

    token = lexer->getNextToken();
    return *token;
}

Token Parser::getAndConsumeToken()
{
    Token token = getToken();
    consumeToken();

    return token;
}

const void Parser::consumeToken()
{
    token = std::nullopt;
}

const bool Parser::consumeTokenIfType(const TokenType& type)
{
    if (checkCurTokenType(type))
    {
        consumeToken();
        return true;
    }

    return false;
}

const bool Parser::consumeTokenIfType(const std::vector<TokenType>& types)
{
    if (checkCurTokenType(types))
    {
        consumeToken();
        return true;
    }

    return false;
}

const bool Parser::consumeTokenIfType_Otherwise_AddLog(const TokenType& tokenType, const LogType& logType)
{
    if (consumeTokenIfType(tokenType))
        return true;

    logger->newLog(logType, getToken());
    return false;
}

const bool Parser::consumeTokenIfType_Otherwise_AddError(const TokenType& tokenType, const LogType& logType)
{
    if (consumeTokenIfType(tokenType))
        return true;

    logger->addNewError(logType, getToken());
    return false;
}

const bool Parser::checkCurTokenType(const TokenType& type)
{
    return (getToken().type == type);
}

const bool Parser::checkCurTokenType(const std::vector<TokenType>& types)
{
    return checkTokenType(getToken().type, types);
}

const bool Parser::checkTokenType(const TokenType& tokenType, const TokenType& type)
{
    return tokenType == type;
}

const bool Parser::checkTokenType(const TokenType& tokenType, const std::vector<TokenType>& types)
{
    return (std::find(types.cbegin(), types.cend(), tokenType) != types.cend());
}


