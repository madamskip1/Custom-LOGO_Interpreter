#include "SourceReader.h"
#include "Lexer.h"
#include "Parser.h"
#include "Context.h"
#include "../AST/IfStatement.h"
#include "../AST/RepeatStatement.h"
#include "../AST/RepeatTimeStatement.h"
#include "../AST/Expression.h"
#include "../AST/Condition.h"
#include "../AST/Number.h"
#include "../AST/Boolean.h"
#include "../AST/DefFuncStatement.h"
#include "../AST/CallFuncStatement.h"
#include "../AST/AssignmentStatement.h"
#include "../AST/VariableExpression.h"
#include "../AST/Color.h"

#pragma warning(push, 0)        
#include "catch.hpp"
#pragma warning(pop)

/*
 *	This test scenerios were depreced
 *	*->evaluate() now use Context as parameter (was no arg)
 *	*->evaluate() now return nothing (was return int)
 *	*->evaluate() evaluated value now stored in context as std::variant
 *	
 *	TODO: FIX ALL CASSES to new evaluate method
 */

TEST_CASE("IFStatement", "[parser]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);
    Context context;

    SECTION("without else")
    {
        reader.setSourceString("if (!false) {}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::IfStatement);

        AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(firstNode);
        AST::Condition* condition = dynamic_cast<AST::Condition*>(ifStatement->getCondition());
        REQUIRE(condition->getChildrenExpressionSize() == 1);
        AST::Condition* andCondition = dynamic_cast<AST::Condition*>(condition->getChildExpression(0));
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Boolean* boolean = dynamic_cast<AST::Boolean*>(andCondition->getChildExpression(0));
        REQUIRE(condition->getChildrenExpressionSize() == 1);
        REQUIRE(boolean->getNegativeOperator());
        REQUIRE(boolean->getValue());

        AST::InstructionsBlock* trueBlock = ifStatement->getTrueBlockNode();
        REQUIRE(trueBlock->getNodeType() == AST::NodeType::InstructionsBlock);
        REQUIRE(!ifStatement->hasElseBlock());

        condition->evaluate(&context);
        REQUIRE(context.evaluateValue.index() == 3);
        REQUIRE(std::get<bool>(context.evaluateValue) == true);
    }

    SECTION("with else")
    {
        reader.setSourceString("if (true) {} else {}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::IfStatement);

        AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(firstNode);
        AST::Condition* condition = dynamic_cast<AST::Condition*>(ifStatement->getCondition());
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* andCondition = condition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* boolean = andCondition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);
        REQUIRE(!boolean->getNegativeOperator());

        AST::InstructionsBlock* trueBlock = ifStatement->getTrueBlockNode();
        REQUIRE(trueBlock->getNodeType() == AST::NodeType::InstructionsBlock);
        REQUIRE(ifStatement->hasElseBlock());
        AST::InstructionsBlock* elseBlock = ifStatement->getElseBlockNode();
        REQUIRE(trueBlock->getNodeType() == AST::NodeType::InstructionsBlock);

        condition->evaluate(&context);
        REQUIRE(std::get<bool>(context.evaluateValue) == true);
    }
}



TEST_CASE("RepeatStatement", "[parser]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);
    Context context;


    reader.setSourceString("repeat(50) {}");

    std::unique_ptr<AST::Node> rootNode = parser.parse();
    AST::Node* firstNode = rootNode->getChild(0);
    REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatStatement);

    AST::RepeatStatement* repeatStatement = dynamic_cast<AST::RepeatStatement*>(firstNode);

    AST::Expression* condition = repeatStatement->getHowManyTime();
    REQUIRE(condition->getChildrenExpressionSize() == 1);

    AST::Expression* andCondition = condition->getChildExpression(0);
    REQUIRE(condition->getChildrenExpressionSize() == 1);

    AST::Expression* relationCondition = andCondition->getChildExpression(0);
    REQUIRE(condition->getChildrenExpressionSize() == 1);

    AST::Expression* addExpression = relationCondition->getChildExpression(0);
    REQUIRE(addExpression->getChildrenExpressionSize() == 1);

    AST::Expression* multiExpression = addExpression->getChildExpression(0);
    REQUIRE(multiExpression->getChildrenExpressionSize() == 1);

    AST::Expression* factor = multiExpression->getChildExpression(0);
    REQUIRE(factor->getChildrenExpressionSize() == 0);
	
    AST::InstructionsBlock* instructionsBlock = repeatStatement->getInstructuionsBlock();
    REQUIRE(instructionsBlock->getNodeType() == AST::NodeType::InstructionsBlock);

    condition->evaluate(&context);
    REQUIRE(std::get<int>(context.evaluateValue) == 50);
}

TEST_CASE("RepeatTimeStatement", "[parser]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);
    Context context;

    SECTION("only period")
    {
        reader.setSourceString("repeatTime(22) {}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatTimeStatement);

        AST::RepeatTimeStatement* repeatTimeStatement = dynamic_cast<AST::RepeatTimeStatement*>(firstNode);

        AST::Expression* condition = repeatTimeStatement->getPeriod();
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* andCondition = condition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* relationCondition = andCondition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* addExpression = relationCondition->getChildExpression(0);
        REQUIRE(addExpression->getChildrenExpressionSize() == 1);

        AST::Expression* multiExpression = addExpression->getChildExpression(0);
        REQUIRE(multiExpression->getChildrenExpressionSize() == 1);

        AST::Expression* factor = multiExpression->getChildExpression(0);
        REQUIRE(factor->getChildrenExpressionSize() == 0);

        AST::InstructionsBlock* instructionsBlock = repeatTimeStatement->getInstructuionsBlock();
        REQUIRE(instructionsBlock->getNodeType() == AST::NodeType::InstructionsBlock);

        condition->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 22);
    }

    SECTION("period and how many time")
    {
        reader.setSourceString("repeatTime(22, 50) {}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatTimeStatement);

        AST::RepeatTimeStatement* repeatTimeStatement = dynamic_cast<AST::RepeatTimeStatement*>(firstNode);

        AST::Expression* condition = repeatTimeStatement->getPeriod();
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* andCondition = condition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* relationCondition = andCondition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* addExpression = relationCondition->getChildExpression(0);
        REQUIRE(addExpression->getChildrenExpressionSize() == 1);

        AST::Expression* multiExpression = addExpression->getChildExpression(0);
        REQUIRE(multiExpression->getChildrenExpressionSize() == 1);

        AST::Expression* factor = multiExpression->getChildExpression(0);
        REQUIRE(factor->getChildrenExpressionSize() == 0);


        condition->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 22);

        condition = repeatTimeStatement->getHowManyTime();
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        andCondition = condition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        relationCondition = andCondition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        addExpression = relationCondition->getChildExpression(0);
        REQUIRE(addExpression->getChildrenExpressionSize() == 1);

        multiExpression = addExpression->getChildExpression(0);
        REQUIRE(multiExpression->getChildrenExpressionSize() == 1);

        factor = multiExpression->getChildExpression(0);
        REQUIRE(factor->getChildrenExpressionSize() == 0);

        condition->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 50);
    }
}


TEST_CASE("Expressions", "[parser]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);
    Context context;

    SECTION("Simple expression - just digit (zero)")
    {
        reader.setSourceString("repeat(0) {}");
        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatStatement);

        AST::RepeatStatement* repeatStatement = dynamic_cast<AST::RepeatStatement*>(firstNode);
        AST::Expression* condition = repeatStatement->getHowManyTime();
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* andCondition = condition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* relationCondition = andCondition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* addExpression = relationCondition->getChildExpression(0);
        REQUIRE(addExpression->getChildrenExpressionSize() == 1);

        AST::Expression* multiExpression = addExpression->getChildExpression(0);
        REQUIRE(multiExpression->getChildrenExpressionSize() == 1);

        AST::Expression* factor = multiExpression->getChildExpression(0);
        REQUIRE(factor->getChildrenExpressionSize() == 0);


        condition->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 0);

    }
	
    SECTION("Simple expression - negative digit")
    {
        reader.setSourceString("repeat(-95) {}");
        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatStatement);

        AST::RepeatStatement* repeatStatement = dynamic_cast<AST::RepeatStatement*>(firstNode);
        AST::Expression* condition = repeatStatement->getHowManyTime();
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* andCondition = condition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* relationCondition = andCondition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* addExpression = relationCondition->getChildExpression(0);
        REQUIRE(addExpression->getChildrenExpressionSize() == 1);

        AST::Expression* multiExpression = addExpression->getChildExpression(0);
        REQUIRE(multiExpression->getChildrenExpressionSize() == 1);

        AST::Expression* factor = multiExpression->getChildExpression(0);
        REQUIRE(factor->getChildrenExpressionSize() == 0);


        condition->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == -95);
    }

    SECTION("math operation (+, -, *, /) without bracket")
    {
        reader.setSourceString("repeat(-95 + 20 * -20 / -30 - 20) {}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatStatement);

        AST::RepeatStatement* repeatStatement = dynamic_cast<AST::RepeatStatement*>(firstNode);
        AST::Expression* condition = repeatStatement->getHowManyTime();

        REQUIRE(condition->getChildrenExpressionSize() == 1);
        condition = condition->getChildExpression(0);

        REQUIRE(condition->getChildrenExpressionSize() == 1);
        condition = condition->getChildExpression(0);

        AST::Expression* expression = condition->getChildExpression(0);

        //// A + B - C
        REQUIRE(expression->getChildrenExpressionSize() == 3);
        REQUIRE(expression->getOperator(0) == TokenType::Plus);
        REQUIRE(expression->getOperator(1) == TokenType::Minus);

        //// A Term   -95
        AST::Expression* termExpression = expression->getChildExpression(0);
        REQUIRE(termExpression->getChildrenExpressionSize() == 1);
        REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
        AST::Number* number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
        REQUIRE(number->getValue() == 95);
        REQUIRE(number->getNegativeOperator());



        //// B Term  20 * -20 / -30
        termExpression = expression->getChildExpression(1);
        REQUIRE(termExpression->getChildrenExpressionSize() == 3);
        REQUIRE(termExpression->getOperator(0) == TokenType::Multiply);
        REQUIRE(termExpression->getOperator(1) == TokenType::Divide);
		
        // 20 * ...
        REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
        number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
        REQUIRE(number->getValue() == 20);
        REQUIRE(!number->getNegativeOperator());
		
        // ... * -20 / ...
        REQUIRE(termExpression->getChildExpression(1)->getNodeType() == AST::NodeType::Number);
        number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(1));
        REQUIRE(number->getValue() == 20);
        REQUIRE(number->getNegativeOperator());

        // ... / -30
        REQUIRE(termExpression->getChildExpression(2)->getNodeType() == AST::NodeType::Number);
        number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(2));
        REQUIRE(number->getValue() == 30);
        REQUIRE(number->getNegativeOperator());


        // C Term 20
        // Even its  (- 20) its not negation, its substraction
        termExpression = expression->getChildExpression(2);
        REQUIRE(termExpression->getChildrenExpressionSize() == 1);
        REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
        number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
        REQUIRE(number->getValue() == 20);
        REQUIRE(!number->getNegativeOperator());

        expression->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == -102);
    }

    SECTION("expression with brackets")
    {
        reader.setSourceString("repeat(-(1 + -2) * 3) {}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatStatement);

        AST::RepeatStatement* repeatStatement = dynamic_cast<AST::RepeatStatement*>(firstNode);
        AST::Expression* expression = repeatStatement->getHowManyTime();

        expression->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 3);
    }

    SECTION("expression fun call")
    {
        reader.setSourceString("repeat(callFunc(20)) {}");
        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatStatement);

        AST::RepeatStatement* repeatStatement = dynamic_cast<AST::RepeatStatement*>(firstNode);
        AST::Expression* condition = repeatStatement->getHowManyTime();
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* andCondition = condition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* relationCondition = andCondition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        AST::Expression* addExpression = relationCondition->getChildExpression(0);
        REQUIRE(addExpression->getChildrenExpressionSize() == 1);

        AST::Expression* multiExpression = addExpression->getChildExpression(0);
        REQUIRE(multiExpression->getChildrenExpressionSize() == 1);

        AST::Expression* callFunc = multiExpression->getChildExpression(0);
        REQUIRE(callFunc->getChildrenExpressionSize() == 0);

        AST::CallFuncStatement* callFuncStatement = dynamic_cast<AST::CallFuncStatement*>(callFunc);

        condition = callFuncStatement->getArgument(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        andCondition = condition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        relationCondition = andCondition->getChildExpression(0);
        REQUIRE(condition->getChildrenExpressionSize() == 1);

        addExpression = relationCondition->getChildExpression(0);
        REQUIRE(addExpression->getChildrenExpressionSize() == 1);

        multiExpression = addExpression->getChildExpression(0);
        REQUIRE(multiExpression->getChildrenExpressionSize() == 1);

        condition->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 20);
    }

    SECTION("expression identifier")
    {
        reader.setSourceString("repeat(identifier) {}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatStatement);

        AST::RepeatStatement* repeatStatement = dynamic_cast<AST::RepeatStatement*>(firstNode);
        AST::Expression* expression = repeatStatement->getHowManyTime();
        REQUIRE(expression->getChildrenExpressionSize() == 1);

        AST::Expression* termExpression = expression->getChildExpression(0);
        REQUIRE(termExpression->getChildrenExpressionSize() == 1);
    }
}



TEST_CASE("Conditions", "[parser]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);
    Context context;


    SECTION("simple condition - boolean word")
    {
        reader.setSourceString("if(true) {}");
        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::IfStatement);

        AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(firstNode);
        AST::Expression* condition = ifStatement->getCondition();
        REQUIRE(condition->getChildrenExpressionSize() == 1);
        REQUIRE(condition->getChildExpression(0)->getNodeType() == AST::NodeType::Condition);

        AST::Expression* andCondition = condition->getChildExpression(0);
        REQUIRE(andCondition->getChildrenExpressionSize() == 1);
        REQUIRE(andCondition->getChildExpression(0)->getNodeType() == AST::NodeType::Boolean);

        AST::Boolean* boolean = dynamic_cast<AST::Boolean*>(andCondition->getChildExpression(0));
        REQUIRE(boolean->getValue());

        condition->evaluate(&context);
        REQUIRE(std::get<bool>(context.evaluateValue) == true);
    }

    SECTION("simple relation Operator")
    {
        reader.setSourceString("if (2 < -3) {}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::IfStatement);

        AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(firstNode);

        AST::Expression* condition = dynamic_cast<AST::Expression*>(ifStatement->getCondition());
        REQUIRE(condition->getChildrenExpressionSize() == 1);
        AST::Expression* andCondition = condition->getChildExpression(0);
        REQUIRE(andCondition->getChildrenExpressionSize() == 1);
        REQUIRE(andCondition->getChildExpression(0)->getNodeType() == AST::NodeType::Condition);

        AST::Condition* relationCondition = dynamic_cast<AST::Condition*>(andCondition->getChildExpression(0));
        REQUIRE(relationCondition->getChildrenExpressionSize() == 2);
        REQUIRE(relationCondition->getChildExpression(0)->getNodeType() == AST::NodeType::ArithmeticExpression);
        REQUIRE(relationCondition->getChildExpression(1)->getNodeType() == AST::NodeType::ArithmeticExpression);
        REQUIRE(relationCondition->getRelationOperator() == TokenType::Less);

        AST::Expression* expression = relationCondition->getChildExpression(0);
        REQUIRE(expression->getChildrenExpressionSize() == 1);
        AST::Expression* termExpression = expression->getChildExpression(0);
        REQUIRE(termExpression->getChildrenExpressionSize() == 1);
        REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
        AST::Number* number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
        REQUIRE(number->getValue() == 2);
        REQUIRE(!number->getNegativeOperator());

        expression = relationCondition->getChildExpression(1);
        termExpression = expression->getChildExpression(0);
        REQUIRE(termExpression->getChildrenExpressionSize() == 1);
        REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
        number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
        REQUIRE(number->getValue() == 3);
        REQUIRE(number->getNegativeOperator());

        condition->evaluate(&context);
        REQUIRE(std::get<bool>(context.evaluateValue) == false);
    }

    SECTION("AND and OR operators, not ")
    {
        reader.setSourceString("if (!true && false || true) {}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::IfStatement);

        AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(firstNode);
        AST::Expression* condition = dynamic_cast<AST::Expression*>(ifStatement->getCondition());
        REQUIRE(condition->getChildrenExpressionSize() == 2);
        REQUIRE(condition->getChildExpression(0)->getNodeType() == AST::NodeType::Condition);
        REQUIRE(condition->getChildExpression(1)->getNodeType() == AST::NodeType::Condition);

        AST::Expression* andCondition = condition->getChildExpression(0);
        REQUIRE(andCondition->getChildrenExpressionSize() == 2);
        REQUIRE(andCondition->getChildExpression(0)->getNodeType() == AST::NodeType::Boolean);
        REQUIRE(andCondition->getChildExpression(1)->getNodeType() == AST::NodeType::Boolean);

        AST::Boolean* boolean = dynamic_cast<AST::Boolean*>(andCondition->getChildExpression(0));
        REQUIRE(!boolean->getValue());
        boolean = dynamic_cast<AST::Boolean*>(andCondition->getChildExpression(1));
        REQUIRE(!boolean->getValue());
		
        andCondition = dynamic_cast<AST::Condition*>(condition->getChildExpression(1));
        REQUIRE(andCondition->getChildrenExpressionSize() == 1);
        REQUIRE(andCondition->getChildExpression(0)->getNodeType() == AST::NodeType::Boolean);

        boolean = dynamic_cast<AST::Boolean*>(andCondition->getChildExpression(0));
        REQUIRE(boolean->getValue());

        condition->evaluate(&context);
        REQUIRE(std::get<bool>(context.evaluateValue) == true);
    }
}



TEST_CASE("def function", "[parser]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);
    Context context;

    SECTION("simple function, without parameters, without return type")
    {
        reader.setSourceString("function test() {}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::DefFuncStatement);

        AST::DefFuncStatement* defFunction = dynamic_cast<AST::DefFuncStatement*>(firstNode);
        REQUIRE(defFunction->getName() == "test");
        REQUIRE(defFunction->getParametersSize() == 0);
        REQUIRE(!defFunction->hasReturnType());
    }

    SECTION("function with one parameters, without return type")
    {
        reader.setSourceString("function test2(Turtle zolw) {}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::DefFuncStatement);

        AST::DefFuncStatement* defFunction = dynamic_cast<AST::DefFuncStatement*>(firstNode);
        REQUIRE(defFunction->getName() == "test2");
        REQUIRE(defFunction->getParametersSize() == 1);
        REQUIRE(!defFunction->hasReturnType());

        AST::Parameter* parameter = defFunction->getParameter(0);
        REQUIRE(parameter->getType() == TokenType::Turtle);
        REQUIRE(parameter->getName() == "zolw");
    }

    SECTION("function with multi parameters, witout return type")
    {
        reader.setSourceString("function test3(Turtle zolw, Color color, Integer int) {}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::DefFuncStatement);

        AST::DefFuncStatement* defFunction = dynamic_cast<AST::DefFuncStatement*>(firstNode);
        REQUIRE(defFunction->getName() == "test3");
        REQUIRE(defFunction->getParametersSize() == 3);
        REQUIRE(!defFunction->hasReturnType());

        AST::Parameter* parameter = defFunction->getParameter(0);
        REQUIRE(parameter->getType() == TokenType::Turtle);
        REQUIRE(parameter->getName() == "zolw");
		
        parameter = defFunction->getParameter(1);
        REQUIRE(parameter->getType() == TokenType::ColorVar);
        REQUIRE(parameter->getName() == "color");

        parameter = defFunction->getParameter(2);
        REQUIRE(parameter->getType() == TokenType::Integer);
        REQUIRE(parameter->getName() == "int");
    }

    SECTION("function without parameters, with return type")
    {
        reader.setSourceString("Turtle function test4() { Turtle test; return test;}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::DefFuncStatement);

        AST::DefFuncStatement* defFunction = dynamic_cast<AST::DefFuncStatement*>(firstNode);
        REQUIRE(defFunction->getName() == "test4");
        REQUIRE(defFunction->getParametersSize() == 0);
        REQUIRE(defFunction->hasReturnType());
        REQUIRE(defFunction->getReturnType() == TokenType::Turtle);
        REQUIRE(defFunction->getInstructionsBlock() != nullptr);

        AST::InstructionsBlock* instructionsBlock = defFunction->getInstructionsBlock();
        REQUIRE(instructionsBlock->getChildrenSize() == 2);
        REQUIRE(instructionsBlock->getChild(1)->getNodeType() == AST::NodeType::ReturnStatement);

        AST::ReturnStatement* returnStatement = dynamic_cast<AST::ReturnStatement*>(instructionsBlock->getChild(1));
        AST::Expression* expression = dynamic_cast<AST::Expression*>(returnStatement->getReturn());
        REQUIRE(expression->getChildrenExpressionSize() == 1);

        AST::Expression* termExpression = expression->getChildExpression(0);
        REQUIRE(termExpression->getChildrenExpressionSize() == 1);
    }

    SECTION("function witn parameters, with return type")
    {
        reader.setSourceString("Integer function test5(Point point, Integer int) {}");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::DefFuncStatement);

        AST::DefFuncStatement* defFunction = dynamic_cast<AST::DefFuncStatement*>(firstNode);
        REQUIRE(defFunction->getName() == "test5");
        REQUIRE(defFunction->getParametersSize() == 2);
        REQUIRE(defFunction->hasReturnType());
        REQUIRE(defFunction->getReturnType() == TokenType::Integer);

        AST::Parameter* parameter = defFunction->getParameter(0);
        REQUIRE(parameter->getType() == TokenType::Point);
        REQUIRE(parameter->getName() == "point");

        parameter = defFunction->getParameter(1);
        REQUIRE(parameter->getType() == TokenType::Integer);
        REQUIRE(parameter->getName() == "int");
    }
}

TEST_CASE("call function", "[parser]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);
    Context context;

    SECTION("simple call function")
    {
        reader.setSourceString("test();");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::CallFuncStatement);

        AST::CallFuncStatement* callFunc = dynamic_cast<AST::CallFuncStatement*>(firstNode);
        REQUIRE(callFunc->getArgumentsSize() == 0);
        REQUIRE(callFunc->getIdentifiersSize() == 1);
        REQUIRE(callFunc->getIdentifier(0) == "test");
    }

    SECTION("call function with one argument, int")
    {
        reader.setSourceString("test2(153);");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::CallFuncStatement);

        AST::CallFuncStatement* callFunc = dynamic_cast<AST::CallFuncStatement*>(firstNode);
        REQUIRE(callFunc->getArgumentsSize() == 1);
        REQUIRE(callFunc->getIdentifiersSize() == 1);
        REQUIRE(callFunc->getIdentifier(0) == "test2");


        AST::Expression* expression = callFunc->getArgument(0);
        REQUIRE(expression->getChildrenExpressionSize() == 1);

        AST::Expression* expressionTerm = expression->getChildExpression(0);
        REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);

        expression->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 153);
    }

    SECTION("call function with more arguments")
    {
        reader.setSourceString("test3(160, -20, 10);");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::CallFuncStatement);

        AST::CallFuncStatement* callFunc = dynamic_cast<AST::CallFuncStatement*>(firstNode);
        REQUIRE(callFunc->getArgumentsSize() == 3);
        REQUIRE(callFunc->getIdentifiersSize() == 1);
        REQUIRE(callFunc->getIdentifier(0) == "test3");

        AST::Expression* arg;
        AST::Expression* expressionTerm;
        AST::Number* argNumber;

        arg = callFunc->getArgument(0);
        REQUIRE(arg->getChildrenExpressionSize() == 1);
        expressionTerm = arg->getChildExpression(0);
        REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);

        arg->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 160);

        arg = callFunc->getArgument(1);
        REQUIRE(arg->getChildrenExpressionSize() == 1);
        expressionTerm = arg->getChildExpression(0);
        REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);

        arg->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == -20);

        arg = callFunc->getArgument(2);
        REQUIRE(arg->getChildrenExpressionSize() == 1);
        expressionTerm = arg->getChildExpression(0);
        REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);

        arg->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 10);
    }

    SECTION("call function multi level ID")
    {
        reader.setSourceString("test4.test44.test444();");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::CallFuncStatement);

        AST::CallFuncStatement* callFunc = dynamic_cast<AST::CallFuncStatement*>(firstNode);
        REQUIRE(callFunc->getArgumentsSize() == 0);
        REQUIRE(callFunc->getIdentifiersSize() == 3);
        REQUIRE(callFunc->getIdentifier(0) == "test4");
        REQUIRE(callFunc->getIdentifier(1) == "test44");
        REQUIRE(callFunc->getIdentifier(2) == "test444");
    }

    SECTION("call function multi level ID and multi args")
    {
        reader.setSourceString("test5.test55.test555(1, -2, 3);");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::CallFuncStatement);

        AST::CallFuncStatement* callFunc = dynamic_cast<AST::CallFuncStatement*>(firstNode);
        REQUIRE(callFunc->getArgumentsSize() == 3);
        REQUIRE(callFunc->getIdentifiersSize() == 3);
        REQUIRE(callFunc->getIdentifier(0) == "test5");
        REQUIRE(callFunc->getIdentifier(1) == "test55");
        REQUIRE(callFunc->getIdentifier(2) == "test555");

        AST::Expression* arg;
        AST::Expression* expressionTerm;
        AST::Number* argNumber;

        arg = callFunc->getArgument(0);
        REQUIRE(arg->getChildrenExpressionSize() == 1);
        expressionTerm = arg->getChildExpression(0);
        REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);

        arg->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 1);

        arg = callFunc->getArgument(1);
        REQUIRE(arg->getChildrenExpressionSize() == 1);
        expressionTerm = arg->getChildExpression(0);
        REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);

        arg->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == -2);

        arg = callFunc->getArgument(2);
        REQUIRE(arg->getChildrenExpressionSize() == 1);
        expressionTerm = arg->getChildExpression(0);
        REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);

        arg->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 3);
    }
}


TEST_CASE("Var declartion", "[parser]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);
    Context context;

    SECTION("simple declare var")
    {
        reader.setSourceString("Integer test;");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::VarDeclare);

        AST::VarDeclare* varDeclare = dynamic_cast<AST::VarDeclare*>(firstNode);
        REQUIRE(varDeclare->getIdentifier() == "test");
        REQUIRE(varDeclare->getAssignment() == nullptr);
        REQUIRE(varDeclare->getClassAssignment() == nullptr);
        REQUIRE(varDeclare->getType() == TokenType::Integer);
    }


    SECTION("def var with assign")
    {
        reader.setSourceString("Integer test2 =  2 + 2;");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::VarDeclare);

        AST::VarDeclare* varDeclare = dynamic_cast<AST::VarDeclare*>(firstNode);
        REQUIRE(varDeclare->getIdentifier() == "test2");
        REQUIRE(varDeclare->getAssignment() != nullptr);
        REQUIRE(varDeclare->getClassAssignment() == nullptr);
        REQUIRE(varDeclare->getType() == TokenType::Integer);

        AST::AssignmentStatement* assignmentStatement = dynamic_cast<AST::AssignmentStatement*>(varDeclare->getAssignment());

        AST::Expression* expression = dynamic_cast<AST::Expression*>(assignmentStatement->getAssign());

        expression->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 4);
    }
}

TEST_CASE("assign", "[parser]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);
    Context context;

    SECTION("simple assign")
    {
        reader.setSourceString("test = -20;");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::AssignmentStatement);

        AST::AssignmentStatement* assignmentStatement = dynamic_cast<AST::AssignmentStatement*>(firstNode);
        REQUIRE(assignmentStatement->getIdentifier(0) == "test");

        AST::Expression* expression = dynamic_cast<AST::Expression*>(assignmentStatement->getAssign());

        expression->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == -20);
    }

    SECTION("multi level id assign and calc Func")
    {
        reader.setSourceString("test2.test22.test222 = callFunc();");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::AssignmentStatement);

        AST::AssignmentStatement* assignmentStatement = dynamic_cast<AST::AssignmentStatement*>(firstNode);
        REQUIRE(assignmentStatement->getIdentifier(0) == "test2");
        REQUIRE(assignmentStatement->getIdentifier(1) == "test22");
        REQUIRE(assignmentStatement->getIdentifier(2) == "test222");

        AST::Expression* expression = dynamic_cast<AST::Expression*>(assignmentStatement->getAssign());
        REQUIRE(expression->getChildrenExpressionSize() == 1);

        AST::Expression* termExpression = expression->getChildExpression(0);
        REQUIRE(termExpression->getChildrenExpressionSize() == 1);
    }

    SECTION("assign boolean")
    {
        reader.setSourceString("testBoolean = true;");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::AssignmentStatement);
		
        AST::AssignmentStatement* assignment = dynamic_cast<AST::AssignmentStatement*>(firstNode);
        REQUIRE(assignment->getIdentifier(0) == "testBoolean");

        assignment->getAssign()->evaluate(&context);
        REQUIRE(std::get<bool>(context.evaluateValue));
    }

    SECTION("assign color")
    {
        reader.setSourceString("testColor = \"#654321\";");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);

        AST::AssignmentStatement* assign = dynamic_cast<AST::AssignmentStatement*>(firstNode);

        AST::Color* color = dynamic_cast<AST::Color*>(assign->getAssign());
        assign->getAssign()->evaluate(&context);
        REQUIRE(std::get<std::string>(context.evaluateValue) == "#654321");
    }
}

TEST_CASE("declare class-type", "[parser]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);
    Context context;

    SECTION("simple declare")
    {
        reader.setSourceString("Point point; Turtle zolw;");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::VarDeclare);

        AST::VarDeclare* varDeclare = dynamic_cast<AST::VarDeclare*>(firstNode);

        REQUIRE(varDeclare->getType() == TokenType::Point);
        REQUIRE(varDeclare->getAssignment() == nullptr);
        REQUIRE(varDeclare->getClassAssignment() == nullptr);
        REQUIRE(varDeclare->getIdentifier() == "point");

        AST::Node* secondNode = rootNode->getChild(1);
        REQUIRE(secondNode->getNodeType() == AST::NodeType::VarDeclare);
        varDeclare = dynamic_cast<AST::VarDeclare*>(secondNode);
        REQUIRE(varDeclare->getType() == TokenType::Turtle);
        REQUIRE(varDeclare->getAssignment() == nullptr);
        REQUIRE(varDeclare->getClassAssignment() == nullptr);
        REQUIRE(varDeclare->getIdentifier() == "zolw");
    }

    SECTION("declare class with one argument")
    {
        reader.setSourceString("Point point(20); Turtle zolw(-10);");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        REQUIRE(rootNode->getChildrenSize() == 2);
        AST::Node* node = rootNode->getChild(0);
        REQUIRE(node->getNodeType() == AST::NodeType::VarDeclare);

        AST::VarDeclare* varDeclare = dynamic_cast<AST::VarDeclare*>(node);
        REQUIRE(varDeclare->getType() == TokenType::Point);
        REQUIRE(varDeclare->getAssignment() == nullptr);
        REQUIRE(varDeclare->getClassAssignment() != nullptr);
        REQUIRE(varDeclare->getIdentifier() == "point");

        AST::ClassAssignment* classAssign = dynamic_cast<AST::ClassAssignment*>(varDeclare->getClassAssignment());
        REQUIRE(classAssign->getExpressionsSize() == 1);
			
        AST::Expression* expression = classAssign->getExpression(0);

        expression->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 20);

        node = rootNode->getChild(1);
        REQUIRE(node->getNodeType() == AST::NodeType::VarDeclare);

        varDeclare = dynamic_cast<AST::VarDeclare*>(node);
        REQUIRE(varDeclare->getType() == TokenType::Turtle);
        REQUIRE(varDeclare->getAssignment() == nullptr);
        REQUIRE(varDeclare->getClassAssignment() != nullptr);
        REQUIRE(varDeclare->getIdentifier() == "zolw");

        classAssign = dynamic_cast<AST::ClassAssignment*>(varDeclare->getClassAssignment());
        REQUIRE(classAssign->getExpressionsSize() == 1);

        expression = classAssign->getExpression(0);

        expression->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == -10);
    }

    SECTION("declare with multi args")
    {
        reader.setSourceString("Point point(20, 10, 0); Turtle zolw(-10, -20);");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        REQUIRE(rootNode->getChildrenSize() == 2);
        AST::Node* node = rootNode->getChild(0);
        REQUIRE(node->getNodeType() == AST::NodeType::VarDeclare);

        AST::VarDeclare* varDeclare = dynamic_cast<AST::VarDeclare*>(node);
        REQUIRE(varDeclare->getType() == TokenType::Point);
        REQUIRE(varDeclare->getAssignment() == nullptr);
        REQUIRE(varDeclare->getClassAssignment() != nullptr);
        REQUIRE(varDeclare->getIdentifier() == "point");

        AST::ClassAssignment* classAssign = dynamic_cast<AST::ClassAssignment*>(varDeclare->getClassAssignment());
        REQUIRE(classAssign->getExpressionsSize() == 3);

        AST::Expression* expression = classAssign->getExpression(0);
        AST::Expression* expressionTerm;

        REQUIRE(expression->getChildrenExpressionSize() == 1);
        expressionTerm = expression->getChildExpression(0);
        REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
        expression->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 20);

        expression = classAssign->getExpression(1);
        REQUIRE(expression->getChildrenExpressionSize() == 1);
        expressionTerm = expression->getChildExpression(0);
        REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);

        expression->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 10);


        expression = classAssign->getExpression(2);
        REQUIRE(expression->getChildrenExpressionSize() == 1);
        expressionTerm = expression->getChildExpression(0);
        REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);


        expression->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 0);

        node = rootNode->getChild(1);
        REQUIRE(node->getNodeType() == AST::NodeType::VarDeclare);

        varDeclare = dynamic_cast<AST::VarDeclare*>(node);
        REQUIRE(varDeclare->getType() == TokenType::Turtle);
        REQUIRE(varDeclare->getAssignment() == nullptr);
        REQUIRE(varDeclare->getClassAssignment() != nullptr);
        REQUIRE(varDeclare->getIdentifier() == "zolw");

        classAssign = dynamic_cast<AST::ClassAssignment*>(varDeclare->getClassAssignment());
        REQUIRE(classAssign->getExpressionsSize() == 2);

        expression = classAssign->getExpression(0);

        REQUIRE(expression->getChildrenExpressionSize() == 1);
        expressionTerm = expression->getChildExpression(0);
        REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		
        expression->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == -10);
		
        expression = classAssign->getExpression(1);
        REQUIRE(expression->getChildrenExpressionSize() == 1);
        expressionTerm = expression->getChildExpression(0);
        REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);

        expression->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == -20);
    }
}

TEST_CASE("color var decl")
{
    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);
    Context context;

    SECTION("simple declare")
    {
        reader.setSourceString("Color col1;");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::VarDeclare);

        AST::VarDeclare* varDeclare = dynamic_cast<AST::VarDeclare*>(firstNode);

        REQUIRE(varDeclare->getType() == TokenType::ColorVar);
        REQUIRE(varDeclare->getAssignment() == nullptr);
        REQUIRE(varDeclare->getClassAssignment() == nullptr);
        REQUIRE(varDeclare->getIdentifier() == "col1");
    }

    SECTION("declare with assign")
    {
        reader.setSourceString("Color col2 = \"#123456\";");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::VarDeclare);

        AST::VarDeclare* varDeclare = dynamic_cast<AST::VarDeclare*>(firstNode);

        REQUIRE(varDeclare->getType() == TokenType::ColorVar);
        REQUIRE(varDeclare->getAssignment() != nullptr);
        REQUIRE(varDeclare->getClassAssignment() == nullptr);
        REQUIRE(varDeclare->getIdentifier() == "col2");

        AST::AssignmentStatement* assign = varDeclare->getAssignment();
        REQUIRE(assign->getAssign() != nullptr);

        assign->getAssign()->evaluate(&context);
        REQUIRE(std::get<std::string>(context.evaluateValue) == "#123456");
    }
}

TEST_CASE("Block of instructions", "[parser]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);
    Context context;

    SECTION("empty block")
    {
        reader.setSourceString(R"(if (true)
{

}
)");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::IfStatement);


        AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(firstNode);
        REQUIRE(ifStatement->getTrueBlockNode() != nullptr);
        REQUIRE(ifStatement->getElseBlockNode() == nullptr);
        REQUIRE(ifStatement->getCondition()->getNodeType() == AST::NodeType::Condition);
		
        AST::Node* trueBlock = ifStatement->getTrueBlockNode();
        REQUIRE(trueBlock->getChildrenSize() == 0);
    }

    SECTION("three instructions in block")
    {
        reader.setSourceString(R"(if (true)
{
    callFunc();
    repeat(20) {}
    Turtle testTurtle;
}
)");

        std::unique_ptr<AST::Node> rootNode = parser.parse();
        AST::Node* firstNode = rootNode->getChild(0);
        REQUIRE(firstNode->getNodeType() == AST::NodeType::IfStatement);


        AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(firstNode);
        REQUIRE(ifStatement->getTrueBlockNode() != nullptr);
        REQUIRE(ifStatement->getElseBlockNode() == nullptr);
        REQUIRE(ifStatement->getCondition()->getNodeType() == AST::NodeType::Condition);

        REQUIRE(ifStatement->getTrueBlockNode()->getChildrenSize() == 3);

        AST::RepeatStatement* repeat = static_cast<AST::RepeatStatement*>(ifStatement->getTrueBlockNode()->getChild(1));
        repeat->getHowManyTime()->evaluate(&context);
        REQUIRE(std::get<int>(context.evaluateValue) == 20);

        AST::Node* trueBlock = ifStatement->getTrueBlockNode();
        REQUIRE(trueBlock->getChildrenSize() == 3);
        REQUIRE(trueBlock->getChild(0)->getNodeType() == AST::NodeType::CallFuncStatement);
        REQUIRE(trueBlock->getChild(1)->getNodeType() == AST::NodeType::RepeatStatement);
        REQUIRE(trueBlock->getChild(2)->getNodeType() == AST::NodeType::VarDeclare);
    }
}

TEST_CASE("Sample Code parsing", "[parser]")
{
    // A test case that checks the correct analysis of sample code that can be used in the program.
    // Includes combinations of different designs

    // Code contains: repeat keyword, digits, identifier, dot/comma/semicolon symbol, string, bracket, relation operators, mathOperators, data types
    std::string code =
        R"(repeat(10) {
    zolw1.go(-30);
}
     // XXXXXX COMMENT

if (-x < -20) {								// XXXXXX COMMENT
    zolw.go(2 + -2);
} else {
    Integer test = 20;
}
Turtle test2;
Point test3(20, 10);
Color test4 = "#123456";
test5 = "#654321";
)";

    SourceReader reader;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);
    Context context;

    reader.setSourceString(code);

    std::unique_ptr<AST::Node> rootNode = parser.parse();
    REQUIRE(rootNode->getChildrenSize() == 6);




    REQUIRE(rootNode->getChild(0)->getNodeType() == AST::NodeType::RepeatStatement);

    AST::RepeatStatement* repeat = dynamic_cast<AST::RepeatStatement*>(rootNode->getChild(0));
    AST::Expression* period = repeat->getHowManyTime();

    period->evaluate(&context);
    REQUIRE(std::get<int>(context.evaluateValue) == 10);

    AST::Node* block = repeat->getInstructuionsBlock();
    REQUIRE(block->getChildrenSize() == 1);
    REQUIRE(block->getChild(0)->getNodeType() == AST::NodeType::CallFuncStatement);

    AST::CallFuncStatement* callFunc = dynamic_cast<AST::CallFuncStatement*>(block->getChild(0));
    REQUIRE(callFunc->getIdentifiersSize() == 2);
    REQUIRE(callFunc->getIdentifier(0) == "zolw1");
    REQUIRE(callFunc->getIdentifier(1) == "go");
    REQUIRE(callFunc->getArgumentsSize() == 1);

    callFunc->getArgument(0)->evaluate(&context);
    REQUIRE(std::get<int>(context.evaluateValue) == -30);



	
    REQUIRE(rootNode->getChild(1)->getNodeType() == AST::NodeType::IfStatement);

    AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(rootNode->getChild(1));

    AST::Expression* condition = ifStatement->getCondition();
    REQUIRE(condition->getChildrenExpressionSize() == 1);
    AST::Expression* andCondition = condition->getChildExpression(0);
    REQUIRE(andCondition->getChildrenExpressionSize() == 1);
    REQUIRE(andCondition->getChildExpression(0)->getNodeType() == AST::NodeType::Condition);

    AST::Expression* relationCondition = andCondition->getChildExpression(0);
    REQUIRE(relationCondition->getChildrenExpressionSize() == 2);

    AST::Expression* expression = relationCondition->getChildExpression(0);
    REQUIRE(expression->getChildrenExpressionSize() == 1);
    AST::Expression* termExpression = expression->getChildExpression(0);
    REQUIRE(termExpression->getChildrenExpressionSize() == 1);
    AST::VariableExpression* variable = dynamic_cast<AST::VariableExpression*>(termExpression->getChildExpression(0));
    REQUIRE(variable->getIdentifier(0) == "x");
    REQUIRE(variable->getNegativeOperator());

    expression = relationCondition->getChildExpression(1);
    termExpression = expression->getChildExpression(0);
    REQUIRE(termExpression->getChildrenExpressionSize() == 1);
    AST::Number* number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
    REQUIRE(number->getValue() == 20);
    REQUIRE(number->getNegativeOperator());

    AST::Node* trueBlock = ifStatement->getTrueBlockNode();
    REQUIRE(trueBlock->getChildrenSize() == 1);
    REQUIRE(trueBlock->getChild(0)->getNodeType() == AST::NodeType::CallFuncStatement);
    callFunc = dynamic_cast<AST::CallFuncStatement*>(trueBlock->getChild(0));
    REQUIRE(callFunc->getIdentifiersSize() == 2);
    REQUIRE(callFunc->getIdentifier(0) == "zolw");
    REQUIRE(callFunc->getIdentifier(1) == "go");
    REQUIRE(callFunc->getArgumentsSize() == 1);
    callFunc->getArgument(0)->evaluate(&context);
    REQUIRE(std::get<int>(context.evaluateValue) == 0);

    AST::Node* elseBlock = ifStatement->getElseBlockNode();
    REQUIRE(elseBlock->getChildrenSize() == 1);
    REQUIRE(elseBlock->getChild(0)->getNodeType() == AST::NodeType::VarDeclare);

    AST::VarDeclare* varDeclare = dynamic_cast<AST::VarDeclare*>(elseBlock->getChild(0));
    REQUIRE(varDeclare->getIdentifier() == "test");
    REQUIRE(varDeclare->getAssignment() != nullptr);
    REQUIRE(varDeclare->getClassAssignment() == nullptr);
    REQUIRE(varDeclare->getType() == TokenType::Integer);

    AST::AssignmentStatement* assignmentStatement = dynamic_cast<AST::AssignmentStatement*>(varDeclare->getAssignment());
    expression = dynamic_cast<AST::Expression*>(assignmentStatement->getAssign());

    expression->evaluate(&context);
    REQUIRE(std::get<int>(context.evaluateValue) == 20);

    REQUIRE(rootNode->getChild(2)->getNodeType() == AST::NodeType::VarDeclare);

    varDeclare = dynamic_cast<AST::VarDeclare*>(rootNode->getChild(2));

    REQUIRE(varDeclare->getType() == TokenType::Turtle);
    REQUIRE(varDeclare->getAssignment() == nullptr);
    REQUIRE(varDeclare->getClassAssignment() == nullptr);
    REQUIRE(varDeclare->getIdentifier() == "test2");




    REQUIRE(rootNode->getChild(3)->getNodeType() == AST::NodeType::VarDeclare);

    varDeclare = dynamic_cast<AST::VarDeclare*>(rootNode->getChild(3));

    REQUIRE(varDeclare->getType() == TokenType::Point);
    REQUIRE(varDeclare->getAssignment() == nullptr);
    REQUIRE(varDeclare->getClassAssignment() != nullptr);
    REQUIRE(varDeclare->getIdentifier() == "test3");

    AST::ClassAssignment* classAssign = dynamic_cast<AST::ClassAssignment*>(varDeclare->getClassAssignment());
    REQUIRE(classAssign->getExpressionsSize() == 2);
    expression = classAssign->getExpression(0);

    expression->evaluate(&context);
    REQUIRE(std::get<int>(context.evaluateValue) == 20);
    expression = classAssign->getExpression(1);

    expression->evaluate(&context);
    REQUIRE(std::get<int>(context.evaluateValue) == 10);



    REQUIRE(rootNode->getChild(4)->getNodeType() == AST::NodeType::VarDeclare);
    varDeclare = dynamic_cast<AST::VarDeclare*>(rootNode->getChild(4));

    REQUIRE(varDeclare->getType() == TokenType::ColorVar);
    REQUIRE(varDeclare->getAssignment() != nullptr);
    REQUIRE(varDeclare->getClassAssignment() == nullptr);
    REQUIRE(varDeclare->getIdentifier() == "test4");

    AST::AssignmentStatement* assign = varDeclare->getAssignment();

    assign->getAssign()->evaluate(&context);
    REQUIRE(std::get<std::string>(context.evaluateValue) == "#123456");




    REQUIRE(rootNode->getChild(5)->getNodeType() == AST::NodeType::AssignmentStatement);

    assign = dynamic_cast<AST::AssignmentStatement*>(rootNode->getChild(5));
    assign->getAssign()->evaluate(&context);
    REQUIRE(std::get<std::string>(context.evaluateValue) == "#654321");

}
