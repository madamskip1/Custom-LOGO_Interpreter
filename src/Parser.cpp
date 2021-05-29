#include "Parser.h"
#include "TokenType.h"

#include "../AST/InstructionsBlock.h"
#include "../AST/IfStatement.h"
#include "../AST/RepeatStatement.h"
#include "../AST/RepeatTimeStatement.h"
#include "../AST/RepeatConditionStatement.h"
#include "../AST/Expression.h"
#include "../AST/ArithmeticExpression.h"
#include "../AST/Number.h"
#include "../AST/Condition.h"
#include "../AST/Boolean.h"
#include "../AST/DefFuncStatement.h"
#include "../AST/CallFuncStatement.h"
#include "../AST/VarDeclare.h"
#include "../AST/VariableExpression.h"
#include "../AST/Color.h"


Parser::Parser(Lexer& lex) : lexer(lex)
{
}

std::unique_ptr<AST::ProgramRootNode> Parser::parse()
{
    return parseProgram();
}

std::unique_ptr<AST::ProgramRootNode> Parser::parseProgram()
{
    std::unique_ptr<AST::ProgramRootNode> rootNode = std::make_unique<AST::ProgramRootNode>();

    std::unique_ptr<AST::Node> node;

    while ((node = parseInstruction()) && !Logger::hasAnyError())
    {
        rootNode->addChild(std::move(node));
    }

    if (!checkCurTokenType(TokenType::EndOfFile))
    {
        Logger::addError(LogType::NotEndOfFile, getToken());
    }

    return rootNode;
}

std::unique_ptr<AST::Node> Parser::parseInstruction()
{
    std::unique_ptr<AST::Node> node;

    node = parseIfStatement();
    if (node)
        return node;

    node = parseRepeatStatement();
    if (node)
        return node;

    node = parseRepeatTimeStatement();
    if (node)
        return node;

    node = parseRepeatConditionStatement();
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

   node = parseDeleteStatement();
   if (node)
       return node;

    return nullptr;
}

std::unique_ptr<AST::InstructionsBlock> Parser::parseInstructionsBlock()
{
    if (!consumeTokenIfType_Otherwise_AddError(TokenType::CurlyBracketOpen, LogType::MissingCurlyBracketOpen))
        return nullptr;

    std::unique_ptr<AST::InstructionsBlock> instructionsBlock = std::make_unique<AST::InstructionsBlock>();

    std::unique_ptr<AST::Node> node;

    Token token = getToken();
    while ((node = parseInstruction()) != nullptr || (node = parseReturnStatement()) != nullptr)
    {
        if (node->getNodeType() == AST::NodeType::DefFuncStatement)
        {
            Logger::addError(LogType::CantDefFuncInBlock, token);
            return nullptr;
        }

        instructionsBlock->addChild(std::move(node));
        token = getToken();
    }

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::CurlyBracketClose, LogType::MissingCurlyBracketClose))
        return nullptr;

    return instructionsBlock;
}

std::unique_ptr<AST::Node> Parser::parseIfStatement()
{
    Token ifToken = getToken();
    if (!consumeTokenIfType(TokenType::If))
        return nullptr;

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketOpen, LogType::MissingRoundBracketOpen))
        return nullptr;

    std::unique_ptr<AST::Expression> condition = parseConditionExpression();

    if (!condition)
        return nullptr;

    /*
        wyj�tek w addError i hasta la vista
    */
    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
        return nullptr;


    std::unique_ptr<AST::InstructionsBlock> trueBlock = parseInstructionsBlock();

    if (!trueBlock)
        return nullptr;

    std::unique_ptr<AST::InstructionsBlock> elseBlock;

    if (consumeTokenIfType(TokenType::Else))
    {
        elseBlock = parseInstructionsBlock();
        if (!elseBlock)
            return nullptr;
    }

    std::unique_ptr<AST::IfStatement> ifStatement = std::make_unique<AST::IfStatement>(std::move(condition), std::move(trueBlock), std::move(elseBlock));
    ifStatement->setToken(ifToken);
    return ifStatement;
}

std::unique_ptr<AST::Node> Parser::parseRepeatStatement()
{
    Token repeatToken = getToken();
    if (!consumeTokenIfType(TokenType::Repeat))
        return nullptr;

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketOpen, LogType::MissingRoundBracketOpen))
        return nullptr;

    std::unique_ptr<AST::Expression> howManyTime = parseExpression();
    if (!howManyTime)
        return nullptr;

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
        return nullptr;

    std::unique_ptr<AST::InstructionsBlock> block = parseInstructionsBlock();
    if (!block)
        return nullptr;

    std::unique_ptr<AST::RepeatStatement> repeatStatement = std::make_unique<AST::RepeatStatement>(std::move(howManyTime), std::move(block));
    repeatStatement->setToken(repeatToken);
    return repeatStatement;
}

std::unique_ptr<AST::Node> Parser::parseRepeatTimeStatement()
{
    Token repeatTimeToken = getToken();
    if (!consumeTokenIfType(TokenType::RepeatTime))
        return nullptr;

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketOpen, LogType::MissingRoundBracketOpen))
        return nullptr;

    std::unique_ptr<AST::Expression> period = parseExpression();
    if (!period)
        return nullptr;

    std::unique_ptr<AST::Expression> howManyTime = nullptr;
    if (consumeTokenIfType(TokenType::Comma))
    {
        howManyTime = parseExpression();
        if (!howManyTime)
            return nullptr;
    }

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
        return nullptr;

    std::unique_ptr<AST::InstructionsBlock> block = parseInstructionsBlock();
    if (!block)
        return nullptr;

    std::unique_ptr<AST::RepeatTimeStatement> repeatTimeStatement;

    if (howManyTime)
    {
        repeatTimeStatement = std::make_unique<AST::RepeatTimeStatement>(std::move(period), std::move(howManyTime), std::move(block));
    }
    else 
    {
        repeatTimeStatement = std::make_unique<AST::RepeatTimeStatement>(std::move(period), std::move(block));
    }
    repeatTimeStatement->setToken(repeatTimeToken);

    return repeatTimeStatement;
}

std::unique_ptr<AST::Node> Parser::parseRepeatConditionStatement()
{
    Token repeatConditionToken = getToken();
    if (!consumeTokenIfType(TokenType::RepeatCondition))
        return nullptr;

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketOpen, LogType::MissingRoundBracketOpen))
        return nullptr;

    std::unique_ptr<AST::Node> condition = parseConditionExpression();

    if (!condition)
        return nullptr;

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
        return nullptr;

    std::unique_ptr<AST::InstructionsBlock> instructionBlock = parseInstructionsBlock();

    std::unique_ptr<AST::RepeatConditionStatement> repeatConditionStatement = std::make_unique<AST::RepeatConditionStatement>(std::move(condition), std::move(instructionBlock));
    repeatConditionStatement->setToken(repeatConditionToken);
    return repeatConditionStatement;
}

std::unique_ptr<AST::Node> Parser::parseAssignOrCallFunctionStatement()
{
    std::vector<std::string> identifiers = parseIdentifiers();

    if (identifiers.empty())
        return nullptr;

    std::unique_ptr<AST::Node> assignOrCall = parseCallFunctionStatement(identifiers);
    if (assignOrCall)
    {
        consumeTokenIfType_Otherwise_AddLog(TokenType::Semicolon, LogType::MissingSemicolon);
        return assignOrCall;
    }

    if (consumeTokenIfType(TokenType::AssignOperator))
    {
        assignOrCall = parseAssignment(identifiers);
        if (!assignOrCall)
            return nullptr;;

        consumeTokenIfType_Otherwise_AddLog(TokenType::Semicolon, LogType::MissingSemicolon);

        return assignOrCall;
    }
    
    return nullptr;

}

std::unique_ptr<AST::CallFuncStatement> Parser::parseCallFunctionStatement(std::vector<std::string> identifiers)
{
    Token callFuncToken = getToken();

    if (!consumeTokenIfType(TokenType::RoundBracketOpen))
        return nullptr;
    
    std::unique_ptr<AST::CallFuncStatement> callFunc = std::make_unique<AST::CallFuncStatement>();

    if (!consumeTokenIfType(TokenType::RoundBracketClose))
    {
        do {
            Token token = getToken();
            std::unique_ptr<AST::Expression> arg = parseExpression();
            if (!arg)
            {
                Logger::addError(LogType::MissingParameter, token);
                return nullptr;
            }

            callFunc->addArgument(std::move(arg));

        } while (consumeTokenIfType(TokenType::Comma));

        if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
            return nullptr;
    }

    callFunc->addIdentifiers(identifiers);
    callFunc->setToken(callFuncToken);
    return callFunc;
}

std::unique_ptr<AST::Node> Parser::parseVarDeclareORDefFuncWithReturStatement()
{
    Token token = getToken();
    
    if (!consumeTokenIfType({ { TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean } }))
        return nullptr;

    std::unique_ptr<AST::Node> decVarOrDefFunc;
    decVarOrDefFunc = parseDefFuncStatement(token.type);

    if (decVarOrDefFunc)
    {
        decVarOrDefFunc->setToken(token);
        return decVarOrDefFunc;
    }

    decVarOrDefFunc = parseVarDeclare(token.type);
    if (decVarOrDefFunc)
    {
        decVarOrDefFunc->setToken(token);
        return decVarOrDefFunc;
    }

    return nullptr;
}

std::unique_ptr<AST::VarDeclare> Parser::parseVarDeclare(const TokenType& type)
{
    Token token = getToken();

	if (!consumeTokenIfType_Otherwise_AddError(TokenType::Identifier, LogType::MissingIdentifierOrFunctionKeyword))
		return nullptr;
    std::string identifier = token.getStringValue();

    std::unique_ptr<AST::ClassAssignment> classAssign;
    std::unique_ptr<AST::AssignmentStatement> assign;

    if (checkCurTokenType(TokenType::AssignOperator))
    {
        consumeToken();
        assign = parseAssignment(std::vector<std::string>({ identifier }));
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

    std::unique_ptr<AST::VarDeclare> varDeclare;

    if (classAssign)
    {
        varDeclare = std::make_unique<AST::VarDeclare>(type, identifier, std::move(classAssign));
    }
    else if (assign)
    {
        varDeclare = std::make_unique<AST::VarDeclare>(type, identifier, std::move(assign));
    }
    else
    {
        varDeclare = std::make_unique<AST::VarDeclare>(type, identifier);
    }

    varDeclare->setToken(token);
    return varDeclare;
}

std::unique_ptr<AST::Node> Parser::parseDefFuncStatement(const TokenType& returnType)
{
    Token defFuncToken = getToken();
    if (!consumeTokenIfType(TokenType::Function))
        return nullptr;
    
    if (!consumeTokenIfType_Otherwise_AddError(TokenType::Identifier, LogType::MissingIdentifier))
        return nullptr;

    std::string name = getToken().getStringValue();


    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketOpen, LogType::MissingRoundBracketOpen))
        return nullptr;

    std::unique_ptr<AST::DefFuncStatement> defFunc = std::make_unique<AST::DefFuncStatement>();
    if (!consumeTokenIfType(TokenType::RoundBracketClose))
    {
        do {
            Token token = getToken();
            std::unique_ptr<AST::Parameter> parameter = parseParameter();

            if (!parameter)
            {
                return nullptr;
            }

            defFunc->addParameter(std::move(parameter));
        } while (consumeTokenIfType(TokenType::Comma));
        
        if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
            return nullptr;

    }

    std::unique_ptr<AST::InstructionsBlock> instructionsBlock = parseInstructionsBlock();

    if (!instructionsBlock)
        return nullptr;

    defFunc->setInstructionsBlock(std::move(instructionsBlock));
    defFunc->setName(name);
    defFunc->setReturnType(returnType);
    defFunc->setToken(defFuncToken);
    return defFunc;
}

std::unique_ptr<AST::AssignmentStatement> Parser::parseAssignment(std::vector<std::string> identifiers)
{
    Token token = getToken();
    std::unique_ptr<AST::Assignable> assignable = parseAssignable();

    if (!assignable)
        return nullptr;

    std::unique_ptr<AST::AssignmentStatement> assign = std::make_unique<AST::AssignmentStatement>(identifiers, std::move(assignable));
    assign->setToken(token);
    return assign;
}

std::unique_ptr<AST::Assignable> Parser::parseAssignable()
{
    Token token = getToken();

    std::unique_ptr<AST::Assignable> assignable = parseExpression();
    
    if (assignable)
    {
        assignable->setToken(token);
        return assignable;
    }

    Logger::addError(LogType::UnknownAssignable, token);
    return nullptr;
}

std::unique_ptr<AST::ClassAssignment> Parser::parseClassAssignment()
{
    Token classAssignToken = getToken();
    if (!consumeTokenIfType(TokenType::RoundBracketOpen))
        return nullptr;
    
    if (checkCurTokenType(TokenType::RoundBracketClose))
    {
        Logger::addError(LogType::BadExpression, getToken());
        return nullptr;
    }

    std::unique_ptr<AST::ClassAssignment> classAssign = std::make_unique<AST::ClassAssignment>();

    do {
        Token token = getToken();
        std::unique_ptr<AST::Expression> expression = parseExpression();
        
        if (!expression)
            return nullptr;

        classAssign->addExpression(std::move(expression));

    } while (consumeTokenIfType(TokenType::Comma));

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingCurlyBracketClose))
        return nullptr;

    classAssign->setToken(classAssignToken);
    return classAssign;
}

std::unique_ptr<AST::Parameter> Parser::parseParameter()
{
    TokenType paramType = getToken().type;
    
    if (!consumeTokenIfType({ TokenType::ColorVar, TokenType::Integer, TokenType::Turtle, TokenType::Point, TokenType::Boolean }))
    {
        Logger::addError(LogType::BadSyntaxParameter, getToken());
        return nullptr;
    }

    std::string identifier = getToken().getStringValue();

    if (identifier == "" || !consumeTokenIfType_Otherwise_AddError(TokenType::Identifier, LogType::MissingIdentifier))
        return nullptr;

    std::unique_ptr<AST::Parameter> parameter = std::make_unique<AST::Parameter>(paramType, identifier);

    return parameter;
}

std::unique_ptr<AST::Expression> Parser::parseExpression()
{
    return parseConditionExpression();
}

std::unique_ptr<AST::Expression> Parser::parseConditionExpression()
{
    Token conditionToken = getToken();
    std::unique_ptr<AST::Expression> andCondition = parseAndConditionExpression();
    if (!andCondition)
        return nullptr;

    std::unique_ptr<AST::Condition> condition = std::make_unique<AST::Condition>();
    condition->addChildExpression(std::move(andCondition));

    while (consumeTokenIfType(TokenType::Or))
    {
        andCondition = parseAndConditionExpression();
        if (!andCondition)
            return nullptr;

        condition->addChildExpression(std::move(andCondition));
        condition->setRelationOperator(TokenType::Or);
    }

    condition->setToken(conditionToken);
    return condition;
}

std::unique_ptr<AST::Expression> Parser::parseAndConditionExpression()
{
    Token andConditionToken = getToken();

    std::unique_ptr<AST::Expression> relationCondition = parseRelationConditionExpression();
    if (!relationCondition)
        return nullptr;

    std::unique_ptr<AST::Condition> andCondition = std::make_unique<AST::Condition>();
    andCondition->addChildExpression(std::move(relationCondition));

    while (consumeTokenIfType(TokenType::And))
    {
        relationCondition = parseRelationConditionExpression();
        if (!relationCondition)
            return nullptr;
        andCondition->addChildExpression(std::move(relationCondition));
        andCondition->setRelationOperator(TokenType::And);
    }

    andCondition->setToken(andConditionToken);
    return andCondition;
}

std::unique_ptr<AST::Expression> Parser::parseRelationConditionExpression()
{
    Token relConditionToken = getToken();

    bool notOperator = consumeTokenIfType(TokenType::NotOperator);

    if (checkCurTokenType({ TokenType::True, TokenType::False }))
    {
        bool boolean = getAndConsumeToken().type == TokenType::True;
        std::unique_ptr<AST::Boolean> booleanWord = std::make_unique<AST::Boolean>(boolean);
        booleanWord->setNegativeOp(notOperator);
        booleanWord->setToken(relConditionToken);

        return booleanWord;
    }

    Token token = getToken();
    std::unique_ptr<AST::Expression> expression = parseArithmeticAddExpression();
    if (!expression)
    {
        Logger::addError(LogType::BadCondition, token);
        return nullptr;
    }

    std::unique_ptr<AST::Condition> relationCondition = std::make_unique<AST::Condition>();
    relationCondition->setNegativeOp(notOperator);
    relationCondition->addChildExpression(std::move(expression));

    if (checkCurTokenType({ TokenType::Equal, TokenType::NotEqual, TokenType::Less, TokenType::Greater, TokenType::LessOrEqual, TokenType::GreaterOrEqual }))
    {
        TokenType relOp = getAndConsumeToken().type;
        std::unique_ptr<AST::Expression> secondExpression = parseArithmeticAddExpression();

        if(!secondExpression)
            return nullptr;

        relationCondition->setRelationOperator(relOp);
        relationCondition->addChildExpression(std::move(secondExpression));
    }

    relationCondition->setToken(relConditionToken);
    return relationCondition;
}

std::unique_ptr<AST::Expression> Parser::parseArithmeticAddExpression()
{
    Token addToken = getToken();

    std::unique_ptr<AST::Expression> termExpression = parseArithmeticMultiExpression();
    
    if (!termExpression)
        return nullptr;

    std::unique_ptr<AST::ArithmeticExpression> expression = std::make_unique<AST::ArithmeticExpression>();
    expression->addChildExpression(std::move(termExpression));
    
    Token token = getToken();

    while (consumeTokenIfType({ TokenType::Plus, TokenType::Minus }))
    {
        termExpression = parseArithmeticMultiExpression();
        if (!termExpression)
            return nullptr;

        expression->addChildExpression(std::move(termExpression));
        expression->addOperator(token.type);
        /*
            enum operatorow expression
        */
        token = getToken();
    }

    expression->setToken(addToken);
    return expression;
}

std::unique_ptr<AST::Expression> Parser::parseArithmeticMultiExpression()
{
    Token multiToken = getToken();

    std::unique_ptr<AST::Expression> factorExpression = parseFactorExpression();

    if (!factorExpression)
        return nullptr;

    std::unique_ptr<AST::ArithmeticExpression> termExpression = std::make_unique<AST::ArithmeticExpression>();
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

    termExpression->setToken(multiToken);
    return termExpression;
}

std::unique_ptr<AST::Expression> Parser::parseFactorExpression()
{
    Token factorToken = getToken();

    bool negativeOp = consumeTokenIfType(TokenType::Minus);

    if (checkCurTokenType(TokenType::Digit))
    {
        std::unique_ptr<AST::Number> factorExpression = std::make_unique<AST::Number>(getToken().getIntValue());
        factorExpression->setNegativeOp(negativeOp);
        factorExpression->setToken(factorToken);
        consumeToken();

        return factorExpression;
    }

    if (consumeTokenIfType(TokenType::RoundBracketOpen))
    {
        std::unique_ptr<AST::Expression> factorExpression = parseConditionExpression();
        
        if (!factorExpression)
            return nullptr;

        if (!consumeTokenIfType_Otherwise_AddError(TokenType::RoundBracketClose, LogType::MissingRoundBracketClose))
            return nullptr;

        factorExpression->setNegativeOp(negativeOp);
        factorExpression->setToken(factorToken);
        return factorExpression;
    }

    if (checkCurTokenType(TokenType::Identifier))
    {
        std::vector<std::string> identifiers = parseIdentifiers();

        if (identifiers.empty())
            return nullptr;

        if (checkCurTokenType(TokenType::RoundBracketOpen))
        {
            std::unique_ptr<AST::CallFuncStatement> callFunc = parseCallFunctionStatement(identifiers);
            
            if (!callFunc)
                return nullptr;

            callFunc->setToken(factorToken);
            return callFunc;
        }

        std::unique_ptr<AST::VariableExpression> var = std::make_unique<AST::VariableExpression>(identifiers);
        var->setNegativeOp(negativeOp);
        var->setToken(factorToken);

        return var;
    }
    if (checkCurTokenType(TokenType::ColorValue))
    {
        std::unique_ptr<AST::Color> color = std::make_unique<AST::Color>(getAndConsumeToken().getStringValue());
        color->setToken(factorToken);
        return color;
    }

    Logger::addError(LogType::BadExpression, getToken());
    return nullptr;
}

std::unique_ptr<AST::ReturnStatement> Parser::parseReturnStatement()
{
    Token returnToken = getToken();

    if (!consumeTokenIfType(TokenType::Return))
        return nullptr;

    std::unique_ptr<AST::Assignable> assignable = parseAssignable();

    if (!assignable)
        return nullptr;

    consumeTokenIfType_Otherwise_AddLog(TokenType::Semicolon, LogType::MissingSemicolon);

    std::unique_ptr<AST::ReturnStatement> returnStatement = std::make_unique<AST::ReturnStatement>(std::move(assignable));
    returnStatement->setToken(returnToken);

    return returnStatement;
}

std::unique_ptr<AST::DeleteStatement> Parser::parseDeleteStatement()
{
    Token deleteToken = getToken();

    if (!consumeTokenIfType(TokenType::Delete))
        return nullptr;

    Token token = getToken();

    if (!consumeTokenIfType_Otherwise_AddError(TokenType::Identifier, LogType::DeleteMissingIdentifier))
        return nullptr;

    consumeTokenIfType_Otherwise_AddLog(TokenType::Semicolon, LogType::MissingSemicolon);

    std::unique_ptr<AST::DeleteStatement> delStatement = std::make_unique<AST::DeleteStatement>(token.getStringValue());
    delStatement->setToken(deleteToken);
    return delStatement;
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
            Logger::addError(LogType::MissingIdentifier, getToken());
            return std::vector<std::string>();
        }

    } while (consumeTokenIfType(TokenType::Dot));

    return identifiers;
}



Token Parser::getToken()
{
    if (token)
        return *token;

    token = lexer.getNextToken();
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

const bool Parser::consumeTokenIfType_Otherwise_AddLog(const TokenType& tokenType, LogType logType)
{
    if (consumeTokenIfType(tokenType))
        return true;

    Logger::addLog(logType, getToken());
    return false;
}

const bool Parser::consumeTokenIfType_Otherwise_AddError(const TokenType& tokenType, LogType logType)
{
    if (consumeTokenIfType(tokenType))
        return true;

    Logger::addError(logType, getToken());
    return false;
}

const bool Parser::checkCurTokenType(const TokenType& type)
{
    return getToken().isType(type);
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


