#include "SourceReader.h"
#include "Lexer.h"
#include "Parser.h"
#include "IfStatement.h"
#include "RepeatStatement.h"
#include "RepeatTimeStatement.h"
#include "Expression.h"
#include "Condition.h"
#include "Number.h"
#include "Boolean.h"
#include "DefFuncStatement.h"
#include "CallFuncStatement.h"
#include "AssignmentStatement.h"
#include "Variable.h"
#include "Color.h"

#pragma warning(push, 0)        
#include "catch.hpp"
#pragma warning(pop)


TEST_CASE("IFStatement", "[parser]")
{
	SourceReader reader;
	Lexer lexer(reader);
	Logger logger;
	Parser parser(lexer, logger);

	SECTION("without else")
	{
		reader.setSourceString("if (!false) {}");

		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::IfStatement);

		AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(firstNode);
		AST::Condition* condition = dynamic_cast<AST::Condition*>(ifStatement->getCondition());
		REQUIRE(condition->getLeftCondition() != nullptr);
		REQUIRE(condition->getRightCondition() == nullptr);
		AST::Condition* andCondition = dynamic_cast<AST::Condition*>(condition->getLeftCondition());
		REQUIRE(andCondition->getLeftCondition() != nullptr);
		REQUIRE(andCondition->getRightCondition() == nullptr);

		AST::Condition* booleanCondition = dynamic_cast<AST::Condition*>(andCondition->getLeftCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(booleanCondition->getNotOperator());

		AST::Boolean* boolean = dynamic_cast<AST::Boolean*>(booleanCondition->getLeftCondition());
		REQUIRE(!boolean->getValue());

		AST::InstructionsBlock* trueBlock = ifStatement->getTrueBlockNode();
		REQUIRE(trueBlock->getNodeType() == AST::NodeType::InstructionsBlock);
		REQUIRE(!ifStatement->hasElseBlock());

		REQUIRE(condition->evaluate() == true);
	}

	SECTION("with else")
	{
		reader.setSourceString("if (true) {} else {}");

		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::IfStatement);

		AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(firstNode);
		AST::Condition* condition = dynamic_cast<AST::Condition*>(ifStatement->getCondition());
		REQUIRE(condition->getLeftCondition() != nullptr);
		REQUIRE(condition->getRightCondition() == nullptr);
		AST::Condition* andCondition = dynamic_cast<AST::Condition*>(condition->getLeftCondition());
		REQUIRE(andCondition->getLeftCondition() != nullptr);
		REQUIRE(andCondition->getRightCondition() == nullptr);

		AST::Condition* booleanCondition = dynamic_cast<AST::Condition*>(andCondition->getLeftCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(!booleanCondition->getNotOperator());

		AST::Boolean* boolean = dynamic_cast<AST::Boolean*>(booleanCondition->getLeftCondition());
		REQUIRE(boolean->getValue());

		AST::InstructionsBlock* trueBlock = ifStatement->getTrueBlockNode();
		REQUIRE(trueBlock->getNodeType() == AST::NodeType::InstructionsBlock);
		REQUIRE(ifStatement->hasElseBlock());
		AST::InstructionsBlock* elseBlock = ifStatement->getElseBlockNode();
		REQUIRE(trueBlock->getNodeType() == AST::NodeType::InstructionsBlock);

		REQUIRE(condition->evaluate() == true);
	}
}



TEST_CASE("RepeatStatement", "[parser]")
{
	SourceReader reader;
	Lexer lexer(reader);
	Logger logger;
	Parser parser(lexer, logger);

	reader.setSourceString("repeat(50) {}");

	std::unique_ptr<AST::Node> rootNode = parser.parse();
	AST::Node* firstNode = rootNode->getChild(0);
	REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatStatement);

	AST::RepeatStatement* repeatStatement = dynamic_cast<AST::RepeatStatement*>(firstNode);

	AST::Expression* expression = repeatStatement->getHowManyTime();
	REQUIRE(expression->getChildrenExpressionSize() == 1);
	AST::Expression* termExpression = expression->getChildExpression(0);
	REQUIRE(termExpression->getChildrenExpressionSize() == 1);
	REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
	AST::Number* number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
	REQUIRE(number->getValue() == 50);
	REQUIRE(!number->getNegativeOperator());
	
	AST::InstructionsBlock* instructionsBlock = repeatStatement->getInstructuionsBlock();
	REQUIRE(instructionsBlock->getNodeType() == AST::NodeType::InstructionsBlock);

	REQUIRE(expression->evaluate() == 50);
}

TEST_CASE("RepeatTimeStatement", "[parser]")
{
	SourceReader reader;
	Lexer lexer(reader);
	Logger logger;
	Parser parser(lexer, logger);

	SECTION("only period")
	{
		reader.setSourceString("repeatTime(22) {}");

		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatTimeStatement);

		AST::RepeatTimeStatement* repeatTimeStatement = dynamic_cast<AST::RepeatTimeStatement*>(firstNode);

		AST::Expression* expression = repeatTimeStatement->getPeriod();
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		AST::Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		AST::Number* number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 22);
		REQUIRE(!number->getNegativeOperator());
		AST::InstructionsBlock* instructionsBlock = repeatTimeStatement->getInstructuionsBlock();
		REQUIRE(instructionsBlock->getNodeType() == AST::NodeType::InstructionsBlock);

		REQUIRE(expression->evaluate() == 22);
	}

	SECTION("period and how many time")
	{
		reader.setSourceString("repeatTime(22, 50) {}");

		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatTimeStatement);

		AST::RepeatTimeStatement* repeatTimeStatement = dynamic_cast<AST::RepeatTimeStatement*>(firstNode);

		AST::Expression* expression = repeatTimeStatement->getPeriod();
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		AST::Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		AST::Number* number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 22);
		REQUIRE(!number->getNegativeOperator());

		REQUIRE(expression->evaluate() == 22);

		expression = repeatTimeStatement->getHowManyTime();
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 50);
		REQUIRE(!number->getNegativeOperator());

		AST::InstructionsBlock* instructionsBlock = repeatTimeStatement->getInstructuionsBlock();
		REQUIRE(instructionsBlock->getNodeType() == AST::NodeType::InstructionsBlock);

		REQUIRE(expression->evaluate() == 50);
	}
}


TEST_CASE("Expressions", "[parser]")
{
	SourceReader reader;
	Lexer lexer(reader);
	Logger logger;
	Parser parser(lexer, logger);

	SECTION("Simple expression - just digit (zero)")
	{
		reader.setSourceString("repeat(0) {}");
		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatStatement);

		AST::RepeatStatement* repeatStatement = dynamic_cast<AST::RepeatStatement*>(firstNode);
		AST::Expression* expression = repeatStatement->getHowManyTime();
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		AST::Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		AST::Number* number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 0);
		REQUIRE(!number->getNegativeOperator());

		REQUIRE(expression->evaluate() == 0);

	}
	
	SECTION("Simple expression - negative digit")
	{
		reader.setSourceString("repeat(-95) {}");
		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatStatement);

		AST::RepeatStatement* repeatStatement = dynamic_cast<AST::RepeatStatement*>(firstNode);
		AST::Expression* expression = repeatStatement->getHowManyTime();
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		AST::Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		AST::Number* number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 95);
		REQUIRE(number->getNegativeOperator());

		REQUIRE(expression->evaluate() == -95);
	}

	SECTION("math operation (+, -, *, /) without bracket")
	{
		reader.setSourceString("repeat(-95 + 20 * -20 / -30 - 20) {}");

		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatStatement);

		AST::RepeatStatement* repeatStatement = dynamic_cast<AST::RepeatStatement*>(firstNode);
		AST::Expression* expression = repeatStatement->getHowManyTime();

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


		REQUIRE(expression->evaluate() == -102);
	}

	SECTION("expression with brackets")
	{
		reader.setSourceString("repeat(-(1 + -2) * 3) {}");

		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatStatement);

		AST::RepeatStatement* repeatStatement = dynamic_cast<AST::RepeatStatement*>(firstNode);
		AST::Expression* expression = repeatStatement->getHowManyTime();

		// A
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		// A = B * C
		AST::Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 2);
		REQUIRE(termExpression->getOperator(0) == TokenType::Multiply);

		// C = 3
		AST::Number* number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(1));
		REQUIRE(number->getValue() == 3);
		REQUIRE(!number->getNegativeOperator());

		// B = D -> bracket exprssion -(1 + -2)
		AST::Expression* factorExpression = termExpression->getChildExpression(0);
		REQUIRE(factorExpression->getNegativeOperator());
		REQUIRE(factorExpression->getChildrenExpressionSize() == 2);

		termExpression = factorExpression->getChildExpression(0);
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 1);
		REQUIRE(!number->getNegativeOperator());
		termExpression = factorExpression->getChildExpression(1);
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 2);
		REQUIRE(number->getNegativeOperator());

		REQUIRE(expression->evaluate() == 3);
	}

	SECTION("expression fun call")
	{
		reader.setSourceString("repeat(callFunc(20)) {}");
		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::RepeatStatement);

		AST::RepeatStatement* repeatStatement = dynamic_cast<AST::RepeatStatement*>(firstNode);
		AST::Expression* expression = repeatStatement->getHowManyTime();
		REQUIRE(expression->getChildrenExpressionSize() == 1);

		AST::Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::CallFuncStatement);
		AST::CallFuncStatement* callFunc = dynamic_cast<AST::CallFuncStatement*>(termExpression->getChildExpression(0));
		REQUIRE(callFunc->getIdentifiersSize() == 1);
		REQUIRE(callFunc->getIdentifier(0) == "callFunc");
		REQUIRE(callFunc->getArgumentsSize() == 1);

		AST::Expression* callFuncArg = callFunc->getArgument(0);
		REQUIRE(callFuncArg->getChildrenExpressionSize() == 1);

		termExpression = callFuncArg->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);

		AST::Number* expressionFactor = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 20);
		REQUIRE(!expressionFactor->getNegativeOperator());
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
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Variable);
		AST::Variable* variable = dynamic_cast<AST::Variable*>(termExpression->getChildExpression(0));
		REQUIRE(variable->getIdentifier(0) == "identifier");
	}
}



TEST_CASE("Conditions", "[parser]")
{
	SourceReader reader;
	Lexer lexer(reader);
	Logger logger;
	Parser parser(lexer, logger);

	SECTION("simple condition - boolean word")
	{
		reader.setSourceString("if(true) {}");
		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::IfStatement);

		AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(firstNode);
		AST::Condition* condition = dynamic_cast<AST::Condition*>(ifStatement->getCondition());
		REQUIRE(condition->getLeftCondition() != nullptr);
		REQUIRE(condition->getRightCondition() == nullptr);
		REQUIRE(condition->getLeftCondition()->getNodeType() == AST::NodeType::Condition);

		AST::Condition* andCondition = dynamic_cast<AST::Condition*>(condition->getLeftCondition());
		REQUIRE(andCondition->getLeftCondition() != nullptr);
		REQUIRE(andCondition->getRightCondition() == nullptr);
		REQUIRE(andCondition->getLeftCondition()->getNodeType() == AST::NodeType::Condition);

		AST::Condition* booleanCondition = dynamic_cast<AST::Condition*>(andCondition->getLeftCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(!booleanCondition->getNotOperator());
		REQUIRE(booleanCondition->getLeftCondition()->getNodeType() == AST::NodeType::Boolean);

		AST::Boolean* boolean = dynamic_cast<AST::Boolean*>(booleanCondition->getLeftCondition());
		REQUIRE(boolean->getValue());


		REQUIRE(condition->evaluate() == true);
	}



	SECTION("simple relation Operator")
	{
		reader.setSourceString("if (2 < -3) {}");

		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::IfStatement);

		AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(firstNode);
		AST::Condition* condition = dynamic_cast<AST::Condition*>(ifStatement->getCondition());
		REQUIRE(condition->getLeftCondition() != nullptr);
		AST::Condition* andCondition = dynamic_cast<AST::Condition*>(condition->getLeftCondition());
		REQUIRE(andCondition->getLeftCondition() != nullptr);
		REQUIRE(andCondition->getRightCondition() == nullptr);
		REQUIRE(andCondition->getLeftCondition()->getNodeType() == AST::NodeType::Condition);

		AST::Condition* relationCondition = dynamic_cast<AST::Condition*>(andCondition->getLeftCondition());
		REQUIRE(relationCondition->getLeftCondition() != nullptr);
		REQUIRE(relationCondition->getRightCondition() != nullptr);
		REQUIRE(relationCondition->getLeftCondition()->getNodeType() == AST::NodeType::Expression);
		REQUIRE(relationCondition->getRightCondition()->getNodeType() == AST::NodeType::Expression);
		REQUIRE(relationCondition->getRelationOperator() == TokenType::Less);

		AST::Expression* expression = dynamic_cast<AST::Expression*>(relationCondition->getLeftCondition());
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		AST::Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		AST::Number* number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 2);
		REQUIRE(!number->getNegativeOperator());

		expression = dynamic_cast<AST::Expression*>(relationCondition->getRightCondition());
		termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 3);
		REQUIRE(number->getNegativeOperator());

		REQUIRE(condition->evaluate() == false);
	}

	SECTION("AND and OR operators, not ")
	{
		reader.setSourceString("if (!true && false || true) {}");

		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::IfStatement);

		AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(firstNode);
		AST::Condition* condition = dynamic_cast<AST::Condition*>(ifStatement->getCondition());
		REQUIRE(condition->getLeftCondition() != nullptr);
		REQUIRE(condition->getRightCondition() != nullptr);
		REQUIRE(condition->getLeftCondition()->getNodeType() == AST::NodeType::Condition);
		REQUIRE(condition->getRightCondition()->getNodeType() == AST::NodeType::Condition);

		AST::Condition* andCondition = dynamic_cast<AST::Condition*>(condition->getLeftCondition());
		REQUIRE(andCondition->getLeftCondition() != nullptr);
		REQUIRE(andCondition->getRightCondition() != nullptr);
		REQUIRE(andCondition->getLeftCondition()->getNodeType() == AST::NodeType::Condition);
		REQUIRE(andCondition->getRightCondition()->getNodeType() == AST::NodeType::Condition);

		AST::Condition* booleanCondition = dynamic_cast<AST::Condition*>(andCondition->getLeftCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(booleanCondition->getNotOperator());
		REQUIRE(booleanCondition->getLeftCondition()->getNodeType() == AST::NodeType::Boolean);
		AST::Boolean* boolean = dynamic_cast<AST::Boolean*>(booleanCondition->getLeftCondition());
	//	REQUIRE(boolean->getValue());
		
		booleanCondition = dynamic_cast<AST::Condition*>(andCondition->getRightCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(!booleanCondition->getNotOperator());
		REQUIRE(booleanCondition->getLeftCondition()->getNodeType() == AST::NodeType::Boolean);
		boolean = dynamic_cast<AST::Boolean*>(booleanCondition->getLeftCondition());
	//	REQUIRE(!boolean->getValue());
		
		andCondition = dynamic_cast<AST::Condition*>(condition->getRightCondition());
		REQUIRE(andCondition->getLeftCondition() != nullptr);
		REQUIRE(andCondition->getRightCondition() == nullptr);
		REQUIRE(andCondition->getLeftCondition()->getNodeType() == AST::NodeType::Condition);

		booleanCondition = dynamic_cast<AST::Condition*>(andCondition->getLeftCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(!booleanCondition->getNotOperator());
		REQUIRE(booleanCondition->getLeftCondition()->getNodeType() == AST::NodeType::Boolean);
		boolean = dynamic_cast<AST::Boolean*>(andCondition->getLeftCondition());
	//	REQUIRE(boolean->getValue());

		REQUIRE(condition->evaluate() == true);
	}

	SECTION("Brackets")
	{
		reader.setSourceString("if (!(callFunc() || 2 == -2) && true) {}");

		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::IfStatement);

		AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(firstNode);
		REQUIRE(ifStatement->getCondition() != nullptr);
		AST::Condition* mainCondition = dynamic_cast<AST::Condition*>(ifStatement->getCondition());

		REQUIRE(mainCondition->getLeftCondition() != nullptr);
		REQUIRE(mainCondition->getRightCondition() == nullptr);
		REQUIRE(mainCondition->getLeftCondition()->getNodeType() == AST::NodeType::Condition);
		REQUIRE(!mainCondition->getNotOperator());
		mainCondition = dynamic_cast<AST::Condition*>(mainCondition->getLeftCondition());
		
		REQUIRE(mainCondition->getLeftCondition() != nullptr);
		REQUIRE(mainCondition->getRightCondition() != nullptr);
		REQUIRE(mainCondition->getLeftCondition()->getNodeType() == AST::NodeType::Condition);
		REQUIRE(mainCondition->getRightCondition()->getNodeType() == AST::NodeType::Condition);
		REQUIRE(!mainCondition->getNotOperator());

		AST::Condition* booleanCondition = dynamic_cast<AST::Condition*>(mainCondition->getRightCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(!booleanCondition->getNotOperator());
		REQUIRE(booleanCondition->getLeftCondition()->getNodeType() == AST::NodeType::Boolean);
		AST::Boolean* boolean = dynamic_cast<AST::Boolean*>(booleanCondition->getLeftCondition());
		REQUIRE(boolean->getValue());

		AST::Condition* bracketCondition = dynamic_cast<AST::Condition*>(mainCondition->getLeftCondition());
		REQUIRE(bracketCondition->getLeftCondition() != nullptr);
		REQUIRE(bracketCondition->getRightCondition() != nullptr);
		REQUIRE(bracketCondition->getLeftCondition()->getNodeType() == AST::NodeType::Condition);
		REQUIRE(bracketCondition->getRightCondition()->getNodeType() == AST::NodeType::Condition);
		REQUIRE(bracketCondition->getNotOperator());

		AST::Condition* callFuncCon = dynamic_cast<AST::Condition*>(bracketCondition->getLeftCondition());
		REQUIRE(callFuncCon->getLeftCondition() != nullptr);
		REQUIRE(callFuncCon->getRightCondition() == nullptr);
		REQUIRE(callFuncCon->getLeftCondition()->getNodeType() == AST::NodeType::Condition);

		callFuncCon = dynamic_cast<AST::Condition*>(callFuncCon->getLeftCondition());
		REQUIRE(callFuncCon->getLeftCondition() != nullptr);
		REQUIRE(callFuncCon->getRightCondition() == nullptr);
		REQUIRE(callFuncCon->getLeftCondition()->getNodeType() == AST::NodeType::Expression);
		
		AST::Expression* expression = dynamic_cast<AST::Expression*>(callFuncCon->getLeftCondition());
		REQUIRE(expression->getChildrenExpressionSize() == 1);

		AST::Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::CallFuncStatement);
		AST::CallFuncStatement* callFunc = dynamic_cast<AST::CallFuncStatement*>(termExpression->getChildExpression(0));
		REQUIRE(callFunc->getIdentifiersSize() == 1);
		REQUIRE(callFunc->getIdentifier(0) == "callFunc");
		REQUIRE(callFunc->getArgumentsSize() == 0);

		AST::Condition* equalityCond = dynamic_cast<AST::Condition*>(bracketCondition->getRightCondition());
		REQUIRE(equalityCond->getLeftCondition() != nullptr);
		REQUIRE(equalityCond->getRightCondition() == nullptr);
		REQUIRE(equalityCond->getLeftCondition()->getNodeType() == AST::NodeType::Condition);

		equalityCond = dynamic_cast<AST::Condition*>(equalityCond->getLeftCondition());
		REQUIRE(equalityCond->getLeftCondition() != nullptr);
		REQUIRE(equalityCond->getRightCondition() != nullptr);
		REQUIRE(equalityCond->getLeftCondition()->getNodeType() == AST::NodeType::Expression);
		REQUIRE(equalityCond->getRightCondition()->getNodeType() == AST::NodeType::Expression);
		REQUIRE(equalityCond->getRelationOperator() == TokenType::Equal);

		expression = dynamic_cast<AST::Expression*>(equalityCond->getLeftCondition());
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		AST::Number* number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 2);
		REQUIRE(!number->getNegativeOperator());

		expression = dynamic_cast<AST::Expression*>(equalityCond->getRightCondition());
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		number = dynamic_cast<AST::Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 2);
		REQUIRE(number->getNegativeOperator());
 	}
}



TEST_CASE("def function", "[parser]")
{
	SourceReader reader;
	Lexer lexer(reader);
	Logger logger;
	Parser parser(lexer, logger);

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
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Variable);
		AST::Variable* variable = dynamic_cast<AST::Variable*>(termExpression->getChildExpression(0));
		REQUIRE(variable->getIdentifier(0) == "test");
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

		AST::Number* expressionFactor = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 153);
		REQUIRE(!expressionFactor->getNegativeOperator());

		REQUIRE(expression->evaluate() == 153);
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
		argNumber = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(argNumber->getValue() == 160);
		REQUIRE(!argNumber->getNegativeOperator());

		REQUIRE(arg->evaluate() == 160);

		arg = callFunc->getArgument(1);
		REQUIRE(arg->getChildrenExpressionSize() == 1);
		expressionTerm = arg->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		argNumber = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(argNumber->getValue() == 20);
		REQUIRE(argNumber->getNegativeOperator());

		REQUIRE(arg->evaluate() == -20);

		arg = callFunc->getArgument(2);
		REQUIRE(arg->getChildrenExpressionSize() == 1);
		expressionTerm = arg->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		argNumber = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(argNumber->getValue() == 10);
		REQUIRE(!argNumber->getNegativeOperator());

		REQUIRE(arg->evaluate() == 10);
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
		argNumber = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(argNumber->getValue() == 1);
		REQUIRE(!argNumber->getNegativeOperator());

		REQUIRE(arg->evaluate() == 1);

		arg = callFunc->getArgument(1);
		REQUIRE(arg->getChildrenExpressionSize() == 1);
		expressionTerm = arg->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		argNumber = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(argNumber->getValue() == 2);
		REQUIRE(argNumber->getNegativeOperator());

		REQUIRE(arg->evaluate() == -2);

		arg = callFunc->getArgument(2);
		REQUIRE(arg->getChildrenExpressionSize() == 1);
		expressionTerm = arg->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		argNumber = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(argNumber->getValue() == 3);
		REQUIRE(!argNumber->getNegativeOperator());

		REQUIRE(arg->evaluate() == 3);
	}
}


TEST_CASE("Var declartion", "[parser]")
{
	SourceReader reader;
	Lexer lexer(reader);
	Logger logger;
	Parser parser(lexer, logger);

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

		REQUIRE(expression->evaluate() == 4);
	}
}

TEST_CASE("assign", "[parser]")
{
	SourceReader reader;
	Lexer lexer(reader);
	Logger logger;
	Parser parser(lexer, logger);

	SECTION("simple assign")
	{
		reader.setSourceString("test = -20;");

		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::AssignmentStatement);

		AST::AssignmentStatement* assignmentStatement = dynamic_cast<AST::AssignmentStatement*>(firstNode);
		REQUIRE(assignmentStatement->getIdentifier(0) == "test");
		REQUIRE(assignmentStatement->getAssign()->getNodeType() == AST::NodeType::Expression);

		AST::Expression* expression = dynamic_cast<AST::Expression*>(assignmentStatement->getAssign());
		REQUIRE(expression->evaluate() == -20);
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
		REQUIRE(assignmentStatement->getAssign()->getNodeType() == AST::NodeType::Expression);

		AST::Expression* expression = dynamic_cast<AST::Expression*>(assignmentStatement->getAssign());
		REQUIRE(expression->getChildrenExpressionSize() == 1);

		AST::Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::CallFuncStatement);
		AST::CallFuncStatement* callFunc = dynamic_cast<AST::CallFuncStatement*>(termExpression->getChildExpression(0));
		REQUIRE(callFunc->getIdentifiersSize() == 1);
		REQUIRE(callFunc->getIdentifier(0) == "callFunc");
		REQUIRE(callFunc->getArgumentsSize() == 0);
	}

	SECTION("assign boolean")
	{
		reader.setSourceString("testBoolean = true;");

		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::AssignmentStatement);
		
		AST::AssignmentStatement* assignment = dynamic_cast<AST::AssignmentStatement*>(firstNode);
		REQUIRE(assignment->getIdentifier(0) == "testBoolean");
		REQUIRE(assignment->getAssign()->getNodeType() == AST::NodeType::Boolean);

		AST::Boolean* boolean = dynamic_cast<AST::Boolean*>(assignment->getAssign());
		REQUIRE(boolean->getValue());
	}

	SECTION("assign color")
	{
		reader.setSourceString("testColor = \"#654321\";");

		std::unique_ptr<AST::Node> rootNode = parser.parse();
		AST::Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == AST::NodeType::AssignmentStatement);

		AST::AssignmentStatement* assign = dynamic_cast<AST::AssignmentStatement*>(firstNode);
		REQUIRE(assign->getAssign()->getNodeType() == AST::NodeType::Color);

		AST::Color* color = dynamic_cast<AST::Color*>(assign->getAssign());
		REQUIRE(color->getColor() == "#654321");
	}
}

TEST_CASE("declare class-type", "[parser]")
{
	SourceReader reader;
	Lexer lexer(reader);
	Logger logger;
	Parser parser(lexer, logger);

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
		REQUIRE(classAssign->getExpression(0)->getNodeType() == AST::NodeType::Expression);
			
		AST::Expression* expression = classAssign->getExpression(0);
		AST::Expression* expressionTerm;
		AST::Number* expressionFactor;

		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		expressionFactor = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 20);
		REQUIRE(!expressionFactor->getNegativeOperator());

		REQUIRE(expression->evaluate() == 20);

		node = rootNode->getChild(1);
		REQUIRE(node->getNodeType() == AST::NodeType::VarDeclare);

		varDeclare = dynamic_cast<AST::VarDeclare*>(node);
		REQUIRE(varDeclare->getType() == TokenType::Turtle);
		REQUIRE(varDeclare->getAssignment() == nullptr);
		REQUIRE(varDeclare->getClassAssignment() != nullptr);
		REQUIRE(varDeclare->getIdentifier() == "zolw");

		classAssign = dynamic_cast<AST::ClassAssignment*>(varDeclare->getClassAssignment());
		REQUIRE(classAssign->getExpressionsSize() == 1);
		REQUIRE(classAssign->getExpression(0)->getNodeType() == AST::NodeType::Expression);

		expression = classAssign->getExpression(0);
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		expressionFactor = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 10);
		REQUIRE(expressionFactor->getNegativeOperator());

		REQUIRE(expression->evaluate() == -10);
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
		REQUIRE(classAssign->getExpression(0)->getNodeType() == AST::NodeType::Expression);

		AST::Expression* expression = classAssign->getExpression(0);
		AST::Expression* expressionTerm;
		AST::Number* expressionFactor;

		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		expressionFactor = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 20);
		REQUIRE(!expressionFactor->getNegativeOperator());

		REQUIRE(expression->evaluate() == 20);

		REQUIRE(classAssign->getExpression(1)->getNodeType() == AST::NodeType::Expression);
		expression = classAssign->getExpression(1);
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		expressionFactor = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 10);
		REQUIRE(!expressionFactor->getNegativeOperator());

		REQUIRE(expression->evaluate() == 10);

		REQUIRE(classAssign->getExpression(2)->getNodeType() == AST::NodeType::Expression);

		expression = classAssign->getExpression(2);
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		expressionFactor = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 0);
		REQUIRE(!expressionFactor->getNegativeOperator());

		REQUIRE(expression->evaluate() == 0);


		node = rootNode->getChild(1);
		REQUIRE(node->getNodeType() == AST::NodeType::VarDeclare);

		varDeclare = dynamic_cast<AST::VarDeclare*>(node);
		REQUIRE(varDeclare->getType() == TokenType::Turtle);
		REQUIRE(varDeclare->getAssignment() == nullptr);
		REQUIRE(varDeclare->getClassAssignment() != nullptr);
		REQUIRE(varDeclare->getIdentifier() == "zolw");

		classAssign = dynamic_cast<AST::ClassAssignment*>(varDeclare->getClassAssignment());
		REQUIRE(classAssign->getExpressionsSize() == 2);
		REQUIRE(classAssign->getExpression(0)->getNodeType() == AST::NodeType::Expression);

		expression = classAssign->getExpression(0);

		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		expressionFactor = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 10);
		REQUIRE(expressionFactor->getNegativeOperator());
		
		REQUIRE(expression->evaluate() == -10);
		
		REQUIRE(classAssign->getExpression(1)->getNodeType() == AST::NodeType::Expression);
		expression = classAssign->getExpression(1);
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
		expressionFactor = dynamic_cast<AST::Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 20);
		REQUIRE(expressionFactor->getNegativeOperator());

		REQUIRE(expression->evaluate() == -20);
	}
}

TEST_CASE("color var decl")
{
	SourceReader reader;
	Lexer lexer(reader);
	Logger logger;
	Parser parser(lexer, logger);

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
		REQUIRE(assign->getAssign()->getNodeType() == AST::NodeType::Color);

		AST::Color* color = dynamic_cast<AST::Color*>(assign->getAssign());
		REQUIRE(color->getColor() == "#123456");
	}
}

TEST_CASE("Block of instructions", "[parser]")
{
	SourceReader reader;
	Lexer lexer(reader);
	Logger logger;
	Parser parser(lexer, logger);

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

	reader.setSourceString(code);

	std::unique_ptr<AST::Node> rootNode = parser.parse();
	REQUIRE(rootNode->getChildrenSize() == 6);




	REQUIRE(rootNode->getChild(0)->getNodeType() == AST::NodeType::RepeatStatement);

	AST::RepeatStatement* repeat = dynamic_cast<AST::RepeatStatement*>(rootNode->getChild(0));
	AST::Expression* period = dynamic_cast<AST::Expression*>(repeat->getHowManyTime());
	REQUIRE(period->evaluate() == 10);

	AST::Node* block = repeat->getInstructuionsBlock();
	REQUIRE(block->getChildrenSize() == 1);
	REQUIRE(block->getChild(0)->getNodeType() == AST::NodeType::CallFuncStatement);
	AST::CallFuncStatement* callFunc = dynamic_cast<AST::CallFuncStatement*>(block->getChild(0));
	REQUIRE(callFunc->getIdentifiersSize() == 2);
	REQUIRE(callFunc->getIdentifier(0) == "zolw1");
	REQUIRE(callFunc->getIdentifier(1) == "go");
	REQUIRE(callFunc->getArgumentsSize() == 1);
	REQUIRE(callFunc->getArgument(0)->evaluate() == -30);



	
	REQUIRE(rootNode->getChild(1)->getNodeType() == AST::NodeType::IfStatement);

	AST::IfStatement* ifStatement = dynamic_cast<AST::IfStatement*>(rootNode->getChild(1));

	AST::Condition* condition = dynamic_cast<AST::Condition*>(ifStatement->getCondition());
	REQUIRE(condition->getLeftCondition() != nullptr);
	AST::Condition* andCondition = dynamic_cast<AST::Condition*>(condition->getLeftCondition());
	REQUIRE(andCondition->getLeftCondition() != nullptr);
	REQUIRE(andCondition->getRightCondition() == nullptr);
	REQUIRE(andCondition->getLeftCondition()->getNodeType() == AST::NodeType::Condition);

	AST::Condition* relationCondition = dynamic_cast<AST::Condition*>(andCondition->getLeftCondition());
	REQUIRE(relationCondition->getLeftCondition() != nullptr);
	REQUIRE(relationCondition->getRightCondition() != nullptr);
	REQUIRE(relationCondition->getLeftCondition()->getNodeType() == AST::NodeType::Expression);
	REQUIRE(relationCondition->getRightCondition()->getNodeType() == AST::NodeType::Expression);
	REQUIRE(relationCondition->getRelationOperator() == TokenType::Less);

	AST::Expression* expression = dynamic_cast<AST::Expression*>(relationCondition->getLeftCondition());
	REQUIRE(expression->getChildrenExpressionSize() == 1);
	AST::Expression* termExpression = expression->getChildExpression(0);
	REQUIRE(termExpression->getChildrenExpressionSize() == 1);
	REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Variable);
	AST::Variable* variable = dynamic_cast<AST::Variable*>(termExpression->getChildExpression(0));
	REQUIRE(variable->getIdentifier(0) == "x");
	REQUIRE(variable->getNegativeOperator());

	expression = dynamic_cast<AST::Expression*>(relationCondition->getRightCondition());
	termExpression = expression->getChildExpression(0);
	REQUIRE(termExpression->getChildrenExpressionSize() == 1);
	REQUIRE(termExpression->getChildExpression(0)->getNodeType() == AST::NodeType::Number);
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
	REQUIRE(callFunc->getArgument(0)->evaluate() == 0);

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
	REQUIRE(expression->evaluate() == 20);




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
	REQUIRE(classAssign->getExpression(0)->getNodeType() == AST::NodeType::Expression);
	REQUIRE(classAssign->getExpression(1)->getNodeType() == AST::NodeType::Expression);
	expression = classAssign->getExpression(0);
	REQUIRE(expression->evaluate() == 20);
	expression = classAssign->getExpression(1);
	REQUIRE(expression->evaluate() == 10);




	REQUIRE(rootNode->getChild(4)->getNodeType() == AST::NodeType::VarDeclare);
	varDeclare = dynamic_cast<AST::VarDeclare*>(rootNode->getChild(4));

	REQUIRE(varDeclare->getType() == TokenType::ColorVar);
	REQUIRE(varDeclare->getAssignment() != nullptr);
	REQUIRE(varDeclare->getClassAssignment() == nullptr);
	REQUIRE(varDeclare->getIdentifier() == "test4");

	AST::AssignmentStatement* assign = varDeclare->getAssignment();
	REQUIRE(assign->getAssign()->getNodeType() == AST::NodeType::Color);

	AST::Color* color = dynamic_cast<AST::Color*>(assign->getAssign());
	REQUIRE(color->getColor() == "#123456");




	REQUIRE(rootNode->getChild(5)->getNodeType() == AST::NodeType::AssignmentStatement);

	assign = dynamic_cast<AST::AssignmentStatement*>(rootNode->getChild(5));
	REQUIRE(assign->getAssign()->getNodeType() == AST::NodeType::Color);

	color = dynamic_cast<AST::Color*>(assign->getAssign());
	REQUIRE(color->getColor() == "#654321");

}