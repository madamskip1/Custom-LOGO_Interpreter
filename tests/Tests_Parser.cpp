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

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);

		IfStatement* ifStatement = static_cast<IfStatement*>(firstNode);
		Condition* condition = static_cast<Condition*>(ifStatement->getCondition());
		REQUIRE(condition->getLeftCondition() != nullptr);
		REQUIRE(condition->getRightCondition() == nullptr);
		Condition* andCondition = static_cast<Condition*>(condition->getLeftCondition());
		REQUIRE(andCondition->getLeftCondition() != nullptr);
		REQUIRE(andCondition->getRightCondition() == nullptr);

		Condition* booleanCondition = static_cast<Condition*>(andCondition->getLeftCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(booleanCondition->getNotOperator());

		Boolean* boolean = static_cast<Boolean*>(booleanCondition->getLeftCondition());
		REQUIRE(!boolean->getValue());

		InstructionsBlock* trueBlock = ifStatement->getTrueBlockNode();
		REQUIRE(trueBlock->getNodeType() == NodeType::InstructionsBlock);
		REQUIRE(!ifStatement->hasElseBlock());

		REQUIRE(condition->evaluate() == true);
	}

	SECTION("with else")
	{
		reader.setSourceString("if (true) {} else {}");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);

		IfStatement* ifStatement = static_cast<IfStatement*>(firstNode);
		Condition* condition = static_cast<Condition*>(ifStatement->getCondition());
		REQUIRE(condition->getLeftCondition() != nullptr);
		REQUIRE(condition->getRightCondition() == nullptr);
		Condition* andCondition = static_cast<Condition*>(condition->getLeftCondition());
		REQUIRE(andCondition->getLeftCondition() != nullptr);
		REQUIRE(andCondition->getRightCondition() == nullptr);

		Condition* booleanCondition = static_cast<Condition*>(andCondition->getLeftCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(!booleanCondition->getNotOperator());

		Boolean* boolean = static_cast<Boolean*>(booleanCondition->getLeftCondition());
		REQUIRE(boolean->getValue());

		InstructionsBlock* trueBlock = ifStatement->getTrueBlockNode();
		REQUIRE(trueBlock->getNodeType() == NodeType::InstructionsBlock);
		REQUIRE(ifStatement->hasElseBlock());
		InstructionsBlock* elseBlock = ifStatement->getElseBlockNode();
		REQUIRE(trueBlock->getNodeType() == NodeType::InstructionsBlock);

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

	std::unique_ptr<Node> rootNode = parser.parse();
	Node* firstNode = rootNode->getChild(0);
	REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

	RepeatStatement* repeatStatement = static_cast<RepeatStatement*>(firstNode);

	Expression* expression = repeatStatement->getHowManyTime();
	REQUIRE(expression->getChildrenExpressionSize() == 1);
	Expression* termExpression = expression->getChildExpression(0);
	REQUIRE(termExpression->getChildrenExpressionSize() == 1);
	REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
	Number* number = static_cast<Number*>(termExpression->getChildExpression(0));
	REQUIRE(number->getValue() == 50);
	REQUIRE(!number->getNegativeOperator());
	
	InstructionsBlock* instructionsBlock = repeatStatement->getInstructuionsBlock();
	REQUIRE(instructionsBlock->getNodeType() == NodeType::InstructionsBlock);

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

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::RepeatTimeStatement);

		RepeatTimeStatement* repeatTimeStatement = static_cast<RepeatTimeStatement*>(firstNode);

		Expression* expression = repeatTimeStatement->getPeriod();
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
		Number* number = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 22);
		REQUIRE(!number->getNegativeOperator());
		InstructionsBlock* instructionsBlock = repeatTimeStatement->getInstructuionsBlock();
		REQUIRE(instructionsBlock->getNodeType() == NodeType::InstructionsBlock);

		REQUIRE(expression->evaluate() == 22);
	}

	SECTION("period and how many time")
	{
		reader.setSourceString("repeatTime(22, 50) {}");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::RepeatTimeStatement);

		RepeatTimeStatement* repeatTimeStatement = static_cast<RepeatTimeStatement*>(firstNode);

		Expression* expression = repeatTimeStatement->getPeriod();
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
		Number* number = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 22);
		REQUIRE(!number->getNegativeOperator());

		REQUIRE(expression->evaluate() == 22);

		expression = repeatTimeStatement->getHowManyTime();
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
		number = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 50);
		REQUIRE(!number->getNegativeOperator());

		InstructionsBlock* instructionsBlock = repeatTimeStatement->getInstructuionsBlock();
		REQUIRE(instructionsBlock->getNodeType() == NodeType::InstructionsBlock);

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
		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

		RepeatStatement* repeatStatement = static_cast<RepeatStatement*>(firstNode);
		Expression* expression = repeatStatement->getHowManyTime();
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
		Number* number = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 0);
		REQUIRE(!number->getNegativeOperator());

		REQUIRE(expression->evaluate() == 0);

	}
	
	SECTION("Simple expression - negative digit")
	{
		reader.setSourceString("repeat(-95) {}");
		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

		RepeatStatement* repeatStatement = static_cast<RepeatStatement*>(firstNode);
		Expression* expression = repeatStatement->getHowManyTime();
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
		Number* number = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 95);
		REQUIRE(number->getNegativeOperator());

		REQUIRE(expression->evaluate() == -95);
	}

	SECTION("math operation (+, -, *, /) without bracket")
	{
		reader.setSourceString("repeat(-95 + 20 * -20 / -30 - 20) {}");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

		RepeatStatement* repeatStatement = static_cast<RepeatStatement*>(firstNode);
		Expression* expression = repeatStatement->getHowManyTime();

		//// A + B - C
		REQUIRE(expression->getChildrenExpressionSize() == 3);
		REQUIRE(expression->getOperator(0) == TokenType::Plus);
		REQUIRE(expression->getOperator(1) == TokenType::Minus);

		//// A Term   -95 
		Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
		Number* number = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 95);
		REQUIRE(number->getNegativeOperator());



		//// B Term  20 * -20 / -30
		termExpression = expression->getChildExpression(1);
		REQUIRE(termExpression->getChildrenExpressionSize() == 3);
		REQUIRE(termExpression->getOperator(0) == TokenType::Multiply);
		REQUIRE(termExpression->getOperator(1) == TokenType::Divide);
		
		// 20 * ...
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
		number = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 20);
		REQUIRE(!number->getNegativeOperator());
		
		// ... * -20 / ...
		REQUIRE(termExpression->getChildExpression(1)->getNodeType() == NodeType::Number);
		number = static_cast<Number*>(termExpression->getChildExpression(1));
		REQUIRE(number->getValue() == 20);
		REQUIRE(number->getNegativeOperator());

		// ... / -30 
		REQUIRE(termExpression->getChildExpression(2)->getNodeType() == NodeType::Number);
		number = static_cast<Number*>(termExpression->getChildExpression(2));
		REQUIRE(number->getValue() == 30);
		REQUIRE(number->getNegativeOperator());


		// C Term 20
		// Even its  (- 20) its not negation, its substraction
		termExpression = expression->getChildExpression(2);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
		number = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 20);
		REQUIRE(!number->getNegativeOperator());


		REQUIRE(expression->evaluate() == -102);
	}

	SECTION("expression with brackets")
	{
		reader.setSourceString("repeat(-(1 + -2) * 3) {}");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

		RepeatStatement* repeatStatement = static_cast<RepeatStatement*>(firstNode);
		Expression* expression = repeatStatement->getHowManyTime();

		// A
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		// A = B * C
		Expression * termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 2);
		REQUIRE(termExpression->getOperator(0) == TokenType::Multiply);

		// C = 3
		Number* number = static_cast<Number*>(termExpression->getChildExpression(1));
		REQUIRE(number->getValue() == 3);
		REQUIRE(!number->getNegativeOperator());

		// B = D -> bracket exprssion -(1 + -2)
		Expression* factorExpression = termExpression->getChildExpression(0);
		REQUIRE(factorExpression->getNegativeOperator());
		REQUIRE(factorExpression->getChildrenExpressionSize() == 2);

		termExpression = factorExpression->getChildExpression(0);
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		number = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 1);
		REQUIRE(!number->getNegativeOperator());
		termExpression = factorExpression->getChildExpression(1);
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		number = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 2);
		REQUIRE(number->getNegativeOperator());

		REQUIRE(expression->evaluate() == 3);
	}

	SECTION("expression fun call")
	{
		reader.setSourceString("repeat(callFunc(20)) {}");
		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

		RepeatStatement* repeatStatement = static_cast<RepeatStatement*>(firstNode);
		Expression* expression = repeatStatement->getHowManyTime();
		REQUIRE(expression->getChildrenExpressionSize() == 1);

		Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::CallFuncStatement);
		CallFuncStatement* callFunc = static_cast<CallFuncStatement*>(termExpression->getChildExpression(0));
		REQUIRE(callFunc->getIdentifiersSize() == 1);
		REQUIRE(callFunc->getIdentifier(0) == "callFunc");
		REQUIRE(callFunc->getArgumentsSize() == 1);

		Expression* callFuncArg = callFunc->getArgument(0);
		REQUIRE(callFuncArg->getChildrenExpressionSize() == 1);

		termExpression = callFuncArg->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);

		Number* expressionFactor = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 20);
		REQUIRE(!expressionFactor->getNegativeOperator());
	}

	SECTION("expression identifier")
	{
		reader.setSourceString("repeat(identifier) {}");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

		RepeatStatement* repeatStatement = static_cast<RepeatStatement*>(firstNode);
		Expression* expression = repeatStatement->getHowManyTime();
		REQUIRE(expression->getChildrenExpressionSize() == 1);

		Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Variable);
		Variable* variable = static_cast<Variable*>(termExpression->getChildExpression(0));
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
		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);

		IfStatement* ifStatement = static_cast<IfStatement*>(firstNode);
		Condition* condition = static_cast<Condition*>(ifStatement->getCondition());
		REQUIRE(condition->getLeftCondition() != nullptr);
		REQUIRE(condition->getRightCondition() == nullptr);
		REQUIRE(condition->getLeftCondition()->getNodeType() == NodeType::Condition);

		Condition* andCondition = static_cast<Condition*>(condition->getLeftCondition());
		REQUIRE(andCondition->getLeftCondition() != nullptr);
		REQUIRE(andCondition->getRightCondition() == nullptr);
		REQUIRE(andCondition->getLeftCondition()->getNodeType() == NodeType::Condition);

		Condition* booleanCondition = static_cast<Condition*>(andCondition->getLeftCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(!booleanCondition->getNotOperator());
		REQUIRE(booleanCondition->getLeftCondition()->getNodeType() == NodeType::Boolean);

		Boolean* boolean = static_cast<Boolean*>(booleanCondition->getLeftCondition());
		REQUIRE(boolean->getValue());


		REQUIRE(condition->evaluate() == true);
	}



	SECTION("simple relation Operator")
	{
		reader.setSourceString("if (2 < -3) {}");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);

		IfStatement* ifStatement = static_cast<IfStatement*>(firstNode);
		Condition* condition = static_cast<Condition*>(ifStatement->getCondition());
		REQUIRE(condition->getLeftCondition() != nullptr);
		Condition* andCondition = static_cast<Condition*>(condition->getLeftCondition());
		REQUIRE(andCondition->getLeftCondition() != nullptr);
		REQUIRE(andCondition->getRightCondition() == nullptr);
		REQUIRE(andCondition->getLeftCondition()->getNodeType() == NodeType::Condition);

		Condition* relationCondition = static_cast<Condition*>(andCondition->getLeftCondition());
		REQUIRE(relationCondition->getLeftCondition() != nullptr);
		REQUIRE(relationCondition->getRightCondition() != nullptr);
		REQUIRE(relationCondition->getLeftCondition()->getNodeType() == NodeType::Expression);
		REQUIRE(relationCondition->getRightCondition()->getNodeType() == NodeType::Expression);
		REQUIRE(relationCondition->getRelationOperator() == TokenType::Less);

		Expression* expression = static_cast<Expression*>(relationCondition->getLeftCondition());
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
		Number* number = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 2);
		REQUIRE(!number->getNegativeOperator());

		expression = static_cast<Expression*>(relationCondition->getRightCondition());
		termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
		number = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 3);
		REQUIRE(number->getNegativeOperator());

		REQUIRE(condition->evaluate() == false);
	}

	SECTION("AND and OR operators, not ")
	{
		reader.setSourceString("if (!true && false || true) {}");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);

		IfStatement* ifStatement = static_cast<IfStatement*>(firstNode);
		Condition* condition = static_cast<Condition*>(ifStatement->getCondition());
		REQUIRE(condition->getLeftCondition() != nullptr);
		REQUIRE(condition->getRightCondition() != nullptr);
		REQUIRE(condition->getLeftCondition()->getNodeType() == NodeType::Condition);
		REQUIRE(condition->getRightCondition()->getNodeType() == NodeType::Condition);

		Condition* andCondition = static_cast<Condition*>(condition->getLeftCondition());
		REQUIRE(andCondition->getLeftCondition() != nullptr);
		REQUIRE(andCondition->getRightCondition() != nullptr);
		REQUIRE(andCondition->getLeftCondition()->getNodeType() == NodeType::Condition);
		REQUIRE(andCondition->getRightCondition()->getNodeType() == NodeType::Condition);

		Condition* booleanCondition = static_cast<Condition*>(andCondition->getLeftCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(booleanCondition->getNotOperator());
		REQUIRE(booleanCondition->getLeftCondition()->getNodeType() == NodeType::Boolean);
		Boolean* boolean = static_cast<Boolean*>(booleanCondition->getLeftCondition());
	//	REQUIRE(boolean->getValue());
		
		booleanCondition = static_cast<Condition*>(andCondition->getRightCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(!booleanCondition->getNotOperator());
		REQUIRE(booleanCondition->getLeftCondition()->getNodeType() == NodeType::Boolean);
		boolean = static_cast<Boolean*>(booleanCondition->getLeftCondition());
	//	REQUIRE(!boolean->getValue());
		
		andCondition = static_cast<Condition*>(condition->getRightCondition());
		REQUIRE(andCondition->getLeftCondition() != nullptr);
		REQUIRE(andCondition->getRightCondition() == nullptr);
		REQUIRE(andCondition->getLeftCondition()->getNodeType() == NodeType::Condition);

		booleanCondition = static_cast<Condition*>(andCondition->getLeftCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(!booleanCondition->getNotOperator());
		REQUIRE(booleanCondition->getLeftCondition()->getNodeType() == NodeType::Boolean);
		boolean = static_cast<Boolean*>(andCondition->getLeftCondition());
	//	REQUIRE(boolean->getValue());

		REQUIRE(condition->evaluate() == true);
	}

	SECTION("Brackets")
	{
		reader.setSourceString("if (!(callFunc() || 2 == -2) && true) {}");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);

		IfStatement* ifStatement = static_cast<IfStatement*>(firstNode);
		REQUIRE(ifStatement->getCondition() != nullptr);
		Condition* mainCondition = static_cast<Condition*>(ifStatement->getCondition());

		REQUIRE(mainCondition->getLeftCondition() != nullptr);
		REQUIRE(mainCondition->getRightCondition() == nullptr);
		REQUIRE(mainCondition->getLeftCondition()->getNodeType() == NodeType::Condition);
		REQUIRE(!mainCondition->getNotOperator());
		mainCondition = static_cast<Condition*>(mainCondition->getLeftCondition());
		
		REQUIRE(mainCondition->getLeftCondition() != nullptr);
		REQUIRE(mainCondition->getRightCondition() != nullptr);
		REQUIRE(mainCondition->getLeftCondition()->getNodeType() == NodeType::Condition);
		REQUIRE(mainCondition->getRightCondition()->getNodeType() == NodeType::Condition);
		REQUIRE(!mainCondition->getNotOperator());

		Condition* booleanCondition = static_cast<Condition*>(mainCondition->getRightCondition());
		REQUIRE(booleanCondition->getLeftCondition() != nullptr);
		REQUIRE(booleanCondition->getRightCondition() == nullptr);
		REQUIRE(!booleanCondition->getNotOperator());
		REQUIRE(booleanCondition->getLeftCondition()->getNodeType() == NodeType::Boolean);
		Boolean* boolean = static_cast<Boolean*>(booleanCondition->getLeftCondition());
		REQUIRE(boolean->getValue());

		Condition* bracketCondition = static_cast<Condition*>(mainCondition->getLeftCondition());
		REQUIRE(bracketCondition->getLeftCondition() != nullptr);
		REQUIRE(bracketCondition->getRightCondition() != nullptr);
		REQUIRE(bracketCondition->getLeftCondition()->getNodeType() == NodeType::Condition);
		REQUIRE(bracketCondition->getRightCondition()->getNodeType() == NodeType::Condition);
		REQUIRE(bracketCondition->getNotOperator());

		Condition* callFuncCon = static_cast<Condition*>(bracketCondition->getLeftCondition());
		REQUIRE(callFuncCon->getLeftCondition() != nullptr);
		REQUIRE(callFuncCon->getRightCondition() == nullptr);
		REQUIRE(callFuncCon->getLeftCondition()->getNodeType() == NodeType::Condition);

		callFuncCon = static_cast<Condition*>(callFuncCon->getLeftCondition());
		REQUIRE(callFuncCon->getLeftCondition() != nullptr);
		REQUIRE(callFuncCon->getRightCondition() == nullptr);
		REQUIRE(callFuncCon->getLeftCondition()->getNodeType() == NodeType::Expression);
		
		Expression* expression = static_cast<Expression*>(callFuncCon->getLeftCondition());
		REQUIRE(expression->getChildrenExpressionSize() == 1);

		Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::CallFuncStatement);
		CallFuncStatement* callFunc = static_cast<CallFuncStatement*>(termExpression->getChildExpression(0));
		REQUIRE(callFunc->getIdentifiersSize() == 1);
		REQUIRE(callFunc->getIdentifier(0) == "callFunc");
		REQUIRE(callFunc->getArgumentsSize() == 0);

		Condition* equalityCond = static_cast<Condition*>(bracketCondition->getRightCondition());
		REQUIRE(equalityCond->getLeftCondition() != nullptr);
		REQUIRE(equalityCond->getRightCondition() == nullptr);
		REQUIRE(equalityCond->getLeftCondition()->getNodeType() == NodeType::Condition);

		equalityCond = static_cast<Condition*>(equalityCond->getLeftCondition());
		REQUIRE(equalityCond->getLeftCondition() != nullptr);
		REQUIRE(equalityCond->getRightCondition() != nullptr);
		REQUIRE(equalityCond->getLeftCondition()->getNodeType() == NodeType::Expression);
		REQUIRE(equalityCond->getRightCondition()->getNodeType() == NodeType::Expression);
		REQUIRE(equalityCond->getRelationOperator() == TokenType::Equal);

		expression = static_cast<Expression*>(equalityCond->getLeftCondition());
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
		Number* number = static_cast<Number*>(termExpression->getChildExpression(0));
		REQUIRE(number->getValue() == 2);
		REQUIRE(!number->getNegativeOperator());

		expression = static_cast<Expression*>(equalityCond->getRightCondition());
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
		number = static_cast<Number*>(termExpression->getChildExpression(0));
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

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::DefFuncStatement);

		DefFuncStatement* defFunction = static_cast<DefFuncStatement*>(firstNode);
		REQUIRE(defFunction->getName() == "test");
		REQUIRE(defFunction->getParametersSize() == 0);
		REQUIRE(!defFunction->hasReturnType());
	}

	SECTION("function with one parameters, without return type")
	{
		reader.setSourceString("function test2(Turtle zolw) {}");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::DefFuncStatement);

		DefFuncStatement* defFunction = static_cast<DefFuncStatement*>(firstNode);
		REQUIRE(defFunction->getName() == "test2");
		REQUIRE(defFunction->getParametersSize() == 1);
		REQUIRE(!defFunction->hasReturnType());

		Parameter* parameter = defFunction->getParameter(0);
		REQUIRE(parameter->getType() == TokenType::Turtle);
		REQUIRE(parameter->getName() == "zolw");
	}

	SECTION("function with multi parameters, witout return type")
	{
		reader.setSourceString("function test3(Turtle zolw, Color color, Integer int) {}");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::DefFuncStatement);

		DefFuncStatement* defFunction = static_cast<DefFuncStatement*>(firstNode);
		REQUIRE(defFunction->getName() == "test3");
		REQUIRE(defFunction->getParametersSize() == 3);
		REQUIRE(!defFunction->hasReturnType());

		Parameter* parameter = defFunction->getParameter(0);
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

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::DefFuncStatement);

		DefFuncStatement* defFunction = static_cast<DefFuncStatement*>(firstNode);
		REQUIRE(defFunction->getName() == "test4");
		REQUIRE(defFunction->getParametersSize() == 0);
		REQUIRE(defFunction->hasReturnType());
		REQUIRE(defFunction->getReturnType() == TokenType::Turtle);
		REQUIRE(defFunction->getInstructionsBlock() != nullptr);

		InstructionsBlock* instructionsBlock = defFunction->getInstructionsBlock();
		REQUIRE(instructionsBlock->getChildrenSize() == 2);
		REQUIRE(instructionsBlock->getChild(1)->getNodeType() == NodeType::ReturnStatement);

		ReturnStatement* returnStatement = static_cast<ReturnStatement*>(instructionsBlock->getChild(1));
		Expression* expression = static_cast<Expression*>(returnStatement->getReturn());
		REQUIRE(expression->getChildrenExpressionSize() == 1);

		Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Variable);
		Variable* variable = static_cast<Variable*>(termExpression->getChildExpression(0));
		REQUIRE(variable->getIdentifier(0) == "test");
	}

	SECTION("function witn parameters, with return type")
	{
		reader.setSourceString("Integer function test5(Point point, Integer int) {}");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::DefFuncStatement);

		DefFuncStatement* defFunction = static_cast<DefFuncStatement*>(firstNode);
		REQUIRE(defFunction->getName() == "test5");
		REQUIRE(defFunction->getParametersSize() == 2);
		REQUIRE(defFunction->hasReturnType());
		REQUIRE(defFunction->getReturnType() == TokenType::Integer);

		Parameter* parameter = defFunction->getParameter(0);
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

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::CallFuncStatement);

		CallFuncStatement* callFunc = static_cast<CallFuncStatement*>(firstNode);
		REQUIRE(callFunc->getArgumentsSize() == 0);
		REQUIRE(callFunc->getIdentifiersSize() == 1);
		REQUIRE(callFunc->getIdentifier(0) == "test");
	}

	SECTION("call function with one argument, int")
	{
		reader.setSourceString("test2(153);");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::CallFuncStatement);

		CallFuncStatement* callFunc = static_cast<CallFuncStatement*>(firstNode);
		REQUIRE(callFunc->getArgumentsSize() == 1);
		REQUIRE(callFunc->getIdentifiersSize() == 1);
		REQUIRE(callFunc->getIdentifier(0) == "test2");


		Expression* expression = callFunc->getArgument(0);
		REQUIRE(expression->getChildrenExpressionSize() == 1);

		Expression* expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);

		Number* expressionFactor = static_cast<Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 153);
		REQUIRE(!expressionFactor->getNegativeOperator());

		REQUIRE(expression->evaluate() == 153);
	}

	SECTION("call function with more arguments")
	{
		reader.setSourceString("test3(160, -20, 10);");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::CallFuncStatement);

		CallFuncStatement* callFunc = static_cast<CallFuncStatement*>(firstNode);
		REQUIRE(callFunc->getArgumentsSize() == 3);
		REQUIRE(callFunc->getIdentifiersSize() == 1);
		REQUIRE(callFunc->getIdentifier(0) == "test3");

		Expression* arg;
		Expression* expressionTerm;
		Number* argNumber;

		arg = callFunc->getArgument(0);
		REQUIRE(arg->getChildrenExpressionSize() == 1);
		expressionTerm = arg->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		argNumber = static_cast<Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(argNumber->getValue() == 160);
		REQUIRE(!argNumber->getNegativeOperator());

		REQUIRE(arg->evaluate() == 160);

		arg = callFunc->getArgument(1);
		REQUIRE(arg->getChildrenExpressionSize() == 1);
		expressionTerm = arg->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		argNumber = static_cast<Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(argNumber->getValue() == 20);
		REQUIRE(argNumber->getNegativeOperator());

		REQUIRE(arg->evaluate() == -20);

		arg = callFunc->getArgument(2);
		REQUIRE(arg->getChildrenExpressionSize() == 1);
		expressionTerm = arg->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		argNumber = static_cast<Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(argNumber->getValue() == 10);
		REQUIRE(!argNumber->getNegativeOperator());

		REQUIRE(arg->evaluate() == 10);
	}

	SECTION("call function multi level ID")
	{
		reader.setSourceString("test4.test44.test444();");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::CallFuncStatement);

		CallFuncStatement* callFunc = static_cast<CallFuncStatement*>(firstNode);
		REQUIRE(callFunc->getArgumentsSize() == 0);
		REQUIRE(callFunc->getIdentifiersSize() == 3);
		REQUIRE(callFunc->getIdentifier(0) == "test4");
		REQUIRE(callFunc->getIdentifier(1) == "test44");
		REQUIRE(callFunc->getIdentifier(2) == "test444");
	}

	SECTION("call function multi level ID and multi args")
	{
		reader.setSourceString("test5.test55.test555(1, -2, 3);");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::CallFuncStatement);

		CallFuncStatement* callFunc = static_cast<CallFuncStatement*>(firstNode);
		REQUIRE(callFunc->getArgumentsSize() == 3);
		REQUIRE(callFunc->getIdentifiersSize() == 3);
		REQUIRE(callFunc->getIdentifier(0) == "test5");
		REQUIRE(callFunc->getIdentifier(1) == "test55");
		REQUIRE(callFunc->getIdentifier(2) == "test555");

		Expression* arg;
		Expression* expressionTerm;
		Number* argNumber;

		arg = callFunc->getArgument(0);
		REQUIRE(arg->getChildrenExpressionSize() == 1);
		expressionTerm = arg->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		argNumber = static_cast<Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(argNumber->getValue() == 1);
		REQUIRE(!argNumber->getNegativeOperator());

		REQUIRE(arg->evaluate() == 1);

		arg = callFunc->getArgument(1);
		REQUIRE(arg->getChildrenExpressionSize() == 1);
		expressionTerm = arg->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		argNumber = static_cast<Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(argNumber->getValue() == 2);
		REQUIRE(argNumber->getNegativeOperator());

		REQUIRE(arg->evaluate() == -2);

		arg = callFunc->getArgument(2);
		REQUIRE(arg->getChildrenExpressionSize() == 1);
		expressionTerm = arg->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		argNumber = static_cast<Number*>(expressionTerm->getChildExpression(0));
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

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::VarDeclare);

		VarDeclare* varDeclare = static_cast<VarDeclare*>(firstNode);
		REQUIRE(varDeclare->getIdentifier() == "test");
		REQUIRE(varDeclare->getAssignment() == nullptr);
		REQUIRE(varDeclare->getClassAssignment() == nullptr);
		REQUIRE(varDeclare->getType() == TokenType::Integer);
	}


	SECTION("def var with assign")
	{
		reader.setSourceString("Integer test2 =  2 + 2;");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::VarDeclare);

		VarDeclare* varDeclare = static_cast<VarDeclare*>(firstNode);
		REQUIRE(varDeclare->getIdentifier() == "test2");
		REQUIRE(varDeclare->getAssignment() != nullptr);
		REQUIRE(varDeclare->getClassAssignment() == nullptr);
		REQUIRE(varDeclare->getType() == TokenType::Integer);

		AssignmentStatement* assignmentStatement = static_cast<AssignmentStatement*>(varDeclare->getAssignment());

		Expression* expression = static_cast<Expression*>(assignmentStatement->getAssign());

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

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::AssignmentStatement);

		AssignmentStatement* assignmentStatement = static_cast<AssignmentStatement*>(firstNode);
		REQUIRE(assignmentStatement->getIdentifier(0) == "test");
		REQUIRE(assignmentStatement->getAssign()->getNodeType() == NodeType::Expression);

		Expression* expression = static_cast<Expression*>(assignmentStatement->getAssign());
		REQUIRE(expression->evaluate() == -20);
	}

	SECTION("multi level id assign and calc Func")
	{
		reader.setSourceString("test2.test22.test222 = callFunc();");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::AssignmentStatement);

		AssignmentStatement* assignmentStatement = static_cast<AssignmentStatement*>(firstNode);
		REQUIRE(assignmentStatement->getIdentifier(0) == "test2");
		REQUIRE(assignmentStatement->getIdentifier(1) == "test22");
		REQUIRE(assignmentStatement->getIdentifier(2) == "test222");
		REQUIRE(assignmentStatement->getAssign()->getNodeType() == NodeType::Expression);

		Expression* expression = static_cast<Expression*>(assignmentStatement->getAssign());
		REQUIRE(expression->getChildrenExpressionSize() == 1);

		Expression* termExpression = expression->getChildExpression(0);
		REQUIRE(termExpression->getChildrenExpressionSize() == 1);
		REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::CallFuncStatement);
		CallFuncStatement* callFunc = static_cast<CallFuncStatement*>(termExpression->getChildExpression(0));
		REQUIRE(callFunc->getIdentifiersSize() == 1);
		REQUIRE(callFunc->getIdentifier(0) == "callFunc");
		REQUIRE(callFunc->getArgumentsSize() == 0);
	}

	SECTION("assign boolean")
	{
		reader.setSourceString("testBoolean = true;");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::AssignmentStatement);
		
		AssignmentStatement* assignment = static_cast<AssignmentStatement*>(firstNode);
		REQUIRE(assignment->getIdentifier(0) == "testBoolean");
		REQUIRE(assignment->getAssign()->getNodeType() == NodeType::Boolean);

		Boolean* boolean = static_cast<Boolean*>(assignment->getAssign());
		REQUIRE(boolean->getValue());
	}

	SECTION("assign color")
	{
		reader.setSourceString("testColor = \"#654321\";");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::AssignmentStatement);

		AssignmentStatement* assign = static_cast<AssignmentStatement*>(firstNode);
		REQUIRE(assign->getAssign()->getNodeType() == NodeType::Color);

		Color* color = static_cast<Color*>(assign->getAssign());
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

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::VarDeclare);

		VarDeclare* varDeclare = static_cast<VarDeclare*>(firstNode);

		REQUIRE(varDeclare->getType() == TokenType::Point);
		REQUIRE(varDeclare->getAssignment() == nullptr);
		REQUIRE(varDeclare->getClassAssignment() == nullptr);
		REQUIRE(varDeclare->getIdentifier() == "point");

		Node* secondNode = rootNode->getChild(1);
		REQUIRE(secondNode->getNodeType() == NodeType::VarDeclare);
		varDeclare = static_cast<VarDeclare*>(secondNode);
		REQUIRE(varDeclare->getType() == TokenType::Turtle);
		REQUIRE(varDeclare->getAssignment() == nullptr);
		REQUIRE(varDeclare->getClassAssignment() == nullptr);
		REQUIRE(varDeclare->getIdentifier() == "zolw");
	}

	SECTION("declare class with one argument")
	{
		reader.setSourceString("Point point(20); Turtle zolw(-10);");

		std::unique_ptr<Node> rootNode = parser.parse();
		REQUIRE(rootNode->getChildrenSize() == 2);
		Node* node = rootNode->getChild(0);
		REQUIRE(node->getNodeType() == NodeType::VarDeclare);

		VarDeclare* varDeclare = static_cast<VarDeclare*>(node);
		REQUIRE(varDeclare->getType() == TokenType::Point);
		REQUIRE(varDeclare->getAssignment() == nullptr);
		REQUIRE(varDeclare->getClassAssignment() != nullptr);
		REQUIRE(varDeclare->getIdentifier() == "point");

		ClassAssignment* classAssign = static_cast<ClassAssignment*>(varDeclare->getClassAssignment());
		REQUIRE(classAssign->getExpressionsSize() == 1);
		REQUIRE(classAssign->getExpression(0)->getNodeType() == NodeType::Expression);
			
		Expression* expression = classAssign->getExpression(0);
		Expression * expressionTerm;
		Number* expressionFactor;

		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == NodeType::Number);
		expressionFactor = static_cast<Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 20);
		REQUIRE(!expressionFactor->getNegativeOperator());

		REQUIRE(expression->evaluate() == 20);

		node = rootNode->getChild(1);
		REQUIRE(node->getNodeType() == NodeType::VarDeclare);

		varDeclare = static_cast<VarDeclare*>(node);
		REQUIRE(varDeclare->getType() == TokenType::Turtle);
		REQUIRE(varDeclare->getAssignment() == nullptr);
		REQUIRE(varDeclare->getClassAssignment() != nullptr);
		REQUIRE(varDeclare->getIdentifier() == "zolw");

		classAssign = static_cast<ClassAssignment*>(varDeclare->getClassAssignment());
		REQUIRE(classAssign->getExpressionsSize() == 1);
		REQUIRE(classAssign->getExpression(0)->getNodeType() == NodeType::Expression);

		expression = classAssign->getExpression(0);
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == NodeType::Number);
		expressionFactor = static_cast<Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 10);
		REQUIRE(expressionFactor->getNegativeOperator());

		REQUIRE(expression->evaluate() == -10);
	}

	SECTION("declare with multi args")
	{
		reader.setSourceString("Point point(20, 10, 0); Turtle zolw(-10, -20);");

		std::unique_ptr<Node> rootNode = parser.parse();
		REQUIRE(rootNode->getChildrenSize() == 2);
		Node* node = rootNode->getChild(0);
		REQUIRE(node->getNodeType() == NodeType::VarDeclare);

		VarDeclare* varDeclare = static_cast<VarDeclare*>(node);
		REQUIRE(varDeclare->getType() == TokenType::Point);
		REQUIRE(varDeclare->getAssignment() == nullptr);
		REQUIRE(varDeclare->getClassAssignment() != nullptr);
		REQUIRE(varDeclare->getIdentifier() == "point");

		ClassAssignment* classAssign = static_cast<ClassAssignment*>(varDeclare->getClassAssignment());
		REQUIRE(classAssign->getExpressionsSize() == 3);
		REQUIRE(classAssign->getExpression(0)->getNodeType() == NodeType::Expression);

		Expression* expression = classAssign->getExpression(0);
		Expression* expressionTerm;
		Number* expressionFactor;

		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == NodeType::Number);
		expressionFactor = static_cast<Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 20);
		REQUIRE(!expressionFactor->getNegativeOperator());

		REQUIRE(expression->evaluate() == 20);

		REQUIRE(classAssign->getExpression(1)->getNodeType() == NodeType::Expression);
		expression = classAssign->getExpression(1);
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == NodeType::Number);
		expressionFactor = static_cast<Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 10);
		REQUIRE(!expressionFactor->getNegativeOperator());

		REQUIRE(expression->evaluate() == 10);

		REQUIRE(classAssign->getExpression(2)->getNodeType() == NodeType::Expression);

		expression = classAssign->getExpression(2);
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == NodeType::Number);
		expressionFactor = static_cast<Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 0);
		REQUIRE(!expressionFactor->getNegativeOperator());

		REQUIRE(expression->evaluate() == 0);


		node = rootNode->getChild(1);
		REQUIRE(node->getNodeType() == NodeType::VarDeclare);

		varDeclare = static_cast<VarDeclare*>(node);
		REQUIRE(varDeclare->getType() == TokenType::Turtle);
		REQUIRE(varDeclare->getAssignment() == nullptr);
		REQUIRE(varDeclare->getClassAssignment() != nullptr);
		REQUIRE(varDeclare->getIdentifier() == "zolw");

		classAssign = static_cast<ClassAssignment*>(varDeclare->getClassAssignment());
		REQUIRE(classAssign->getExpressionsSize() == 2);
		REQUIRE(classAssign->getExpression(0)->getNodeType() == NodeType::Expression);

		expression = classAssign->getExpression(0);

		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == NodeType::Number);
		expressionFactor = static_cast<Number*>(expressionTerm->getChildExpression(0));
		REQUIRE(expressionFactor->getValue() == 10);
		REQUIRE(expressionFactor->getNegativeOperator());
		
		REQUIRE(expression->evaluate() == -10);
		
		REQUIRE(classAssign->getExpression(1)->getNodeType() == NodeType::Expression);
		expression = classAssign->getExpression(1);
		REQUIRE(expression->getChildrenExpressionSize() == 1);
		expressionTerm = expression->getChildExpression(0);
		REQUIRE(expressionTerm->getChildrenExpressionSize() == 1);
		REQUIRE(expressionTerm->getChildExpression(0)->getNodeType() == NodeType::Number);
		expressionFactor = static_cast<Number*>(expressionTerm->getChildExpression(0));
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

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::VarDeclare);

		VarDeclare* varDeclare = static_cast<VarDeclare*>(firstNode);

		REQUIRE(varDeclare->getType() == TokenType::ColorVar);
		REQUIRE(varDeclare->getAssignment() == nullptr);
		REQUIRE(varDeclare->getClassAssignment() == nullptr);
		REQUIRE(varDeclare->getIdentifier() == "col1");
	}

	SECTION("declare with assign")
	{
		reader.setSourceString("Color col2 = \"#123456\";");

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::VarDeclare);

		VarDeclare* varDeclare = static_cast<VarDeclare*>(firstNode);

		REQUIRE(varDeclare->getType() == TokenType::ColorVar);
		REQUIRE(varDeclare->getAssignment() != nullptr);
		REQUIRE(varDeclare->getClassAssignment() == nullptr);
		REQUIRE(varDeclare->getIdentifier() == "col2");

		AssignmentStatement* assign = varDeclare->getAssignment();
		REQUIRE(assign->getAssign()->getNodeType() == NodeType::Color);

		Color* color = static_cast<Color*>(assign->getAssign());
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

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);


		IfStatement* ifStatement = static_cast<IfStatement*>(firstNode);
		REQUIRE(ifStatement->getTrueBlockNode() != nullptr);
		REQUIRE(ifStatement->getElseBlockNode() == nullptr);
		REQUIRE(ifStatement->getCondition()->getNodeType() == NodeType::Condition);
		
		Node* trueBlock = ifStatement->getTrueBlockNode();
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

		std::unique_ptr<Node> rootNode = parser.parse();
		Node* firstNode = rootNode->getChild(0);
		REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);


		IfStatement* ifStatement = static_cast<IfStatement*>(firstNode);
		REQUIRE(ifStatement->getTrueBlockNode() != nullptr);
		REQUIRE(ifStatement->getElseBlockNode() == nullptr);
		REQUIRE(ifStatement->getCondition()->getNodeType() == NodeType::Condition);

		Node* trueBlock = ifStatement->getTrueBlockNode();
		REQUIRE(trueBlock->getChildrenSize() == 3);
		REQUIRE(trueBlock->getChild(0)->getNodeType() == NodeType::CallFuncStatement);
		REQUIRE(trueBlock->getChild(1)->getNodeType() == NodeType::RepeatStatement);
		REQUIRE(trueBlock->getChild(2)->getNodeType() == NodeType::VarDeclare);
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

	std::unique_ptr<Node> rootNode = parser.parse();
	REQUIRE(rootNode->getChildrenSize() == 6);




	REQUIRE(rootNode->getChild(0)->getNodeType() == NodeType::RepeatStatement);

	RepeatStatement* repeat = static_cast<RepeatStatement*>(rootNode->getChild(0));
	Expression* period = static_cast<Expression*>(repeat->getHowManyTime());
	REQUIRE(period->evaluate() == 10);

	Node* block = repeat->getInstructuionsBlock();
	REQUIRE(block->getChildrenSize() == 1);
	REQUIRE(block->getChild(0)->getNodeType() == NodeType::CallFuncStatement);
	CallFuncStatement* callFunc = static_cast<CallFuncStatement*>(block->getChild(0));
	REQUIRE(callFunc->getIdentifiersSize() == 2);
	REQUIRE(callFunc->getIdentifier(0) == "zolw1");
	REQUIRE(callFunc->getIdentifier(1) == "go");
	REQUIRE(callFunc->getArgumentsSize() == 1);
	REQUIRE(callFunc->getArgument(0)->evaluate() == -30);



	
	REQUIRE(rootNode->getChild(1)->getNodeType() == NodeType::IfStatement);

	IfStatement* ifStatement = static_cast<IfStatement*>(rootNode->getChild(1));

	Condition* condition = static_cast<Condition*>(ifStatement->getCondition());
	REQUIRE(condition->getLeftCondition() != nullptr);
	Condition* andCondition = static_cast<Condition*>(condition->getLeftCondition());
	REQUIRE(andCondition->getLeftCondition() != nullptr);
	REQUIRE(andCondition->getRightCondition() == nullptr);
	REQUIRE(andCondition->getLeftCondition()->getNodeType() == NodeType::Condition);

	Condition* relationCondition = static_cast<Condition*>(andCondition->getLeftCondition());
	REQUIRE(relationCondition->getLeftCondition() != nullptr);
	REQUIRE(relationCondition->getRightCondition() != nullptr);
	REQUIRE(relationCondition->getLeftCondition()->getNodeType() == NodeType::Expression);
	REQUIRE(relationCondition->getRightCondition()->getNodeType() == NodeType::Expression);
	REQUIRE(relationCondition->getRelationOperator() == TokenType::Less);

	Expression* expression = static_cast<Expression*>(relationCondition->getLeftCondition());
	REQUIRE(expression->getChildrenExpressionSize() == 1);
	Expression* termExpression = expression->getChildExpression(0);
	REQUIRE(termExpression->getChildrenExpressionSize() == 1);
	REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Variable);
	Variable* variable = static_cast<Variable*>(termExpression->getChildExpression(0));
	REQUIRE(variable->getIdentifier(0) == "x");
	REQUIRE(variable->getNegativeOperator());

	expression = static_cast<Expression*>(relationCondition->getRightCondition());
	termExpression = expression->getChildExpression(0);
	REQUIRE(termExpression->getChildrenExpressionSize() == 1);
	REQUIRE(termExpression->getChildExpression(0)->getNodeType() == NodeType::Number);
	Number* number = static_cast<Number*>(termExpression->getChildExpression(0));
	REQUIRE(number->getValue() == 20);
	REQUIRE(number->getNegativeOperator());

	Node* trueBlock = ifStatement->getTrueBlockNode();
	REQUIRE(trueBlock->getChildrenSize() == 1);
	REQUIRE(trueBlock->getChild(0)->getNodeType() == NodeType::CallFuncStatement);
	callFunc = static_cast<CallFuncStatement*>(trueBlock->getChild(0));
	REQUIRE(callFunc->getIdentifiersSize() == 2);
	REQUIRE(callFunc->getIdentifier(0) == "zolw");
	REQUIRE(callFunc->getIdentifier(1) == "go");
	REQUIRE(callFunc->getArgumentsSize() == 1);
	REQUIRE(callFunc->getArgument(0)->evaluate() == 0);

	Node* elseBlock = ifStatement->getElseBlockNode();
	REQUIRE(elseBlock->getChildrenSize() == 1);
	REQUIRE(elseBlock->getChild(0)->getNodeType() == NodeType::VarDeclare);

	VarDeclare* varDeclare = static_cast<VarDeclare*>(elseBlock->getChild(0));
	REQUIRE(varDeclare->getIdentifier() == "test");
	REQUIRE(varDeclare->getAssignment() != nullptr);
	REQUIRE(varDeclare->getClassAssignment() == nullptr);
	REQUIRE(varDeclare->getType() == TokenType::Integer);

	AssignmentStatement* assignmentStatement = static_cast<AssignmentStatement*>(varDeclare->getAssignment());
	expression = static_cast<Expression*>(assignmentStatement->getAssign());
	REQUIRE(expression->evaluate() == 20);




	REQUIRE(rootNode->getChild(2)->getNodeType() == NodeType::VarDeclare);

	varDeclare = static_cast<VarDeclare*>(rootNode->getChild(2));

	REQUIRE(varDeclare->getType() == TokenType::Turtle);
	REQUIRE(varDeclare->getAssignment() == nullptr);
	REQUIRE(varDeclare->getClassAssignment() == nullptr);
	REQUIRE(varDeclare->getIdentifier() == "test2");




	REQUIRE(rootNode->getChild(3)->getNodeType() == NodeType::VarDeclare);

	varDeclare = static_cast<VarDeclare*>(rootNode->getChild(3));

	REQUIRE(varDeclare->getType() == TokenType::Point);
	REQUIRE(varDeclare->getAssignment() == nullptr);
	REQUIRE(varDeclare->getClassAssignment() != nullptr);
	REQUIRE(varDeclare->getIdentifier() == "test3");

	ClassAssignment* classAssign = static_cast<ClassAssignment*>(varDeclare->getClassAssignment());
	REQUIRE(classAssign->getExpressionsSize() == 2);
	REQUIRE(classAssign->getExpression(0)->getNodeType() == NodeType::Expression);
	REQUIRE(classAssign->getExpression(1)->getNodeType() == NodeType::Expression);
	expression = classAssign->getExpression(0);
	REQUIRE(expression->evaluate() == 20);
	expression = classAssign->getExpression(1);
	REQUIRE(expression->evaluate() == 10);




	REQUIRE(rootNode->getChild(4)->getNodeType() == NodeType::VarDeclare);
	varDeclare = static_cast<VarDeclare*>(rootNode->getChild(4));

	REQUIRE(varDeclare->getType() == TokenType::ColorVar);
	REQUIRE(varDeclare->getAssignment() != nullptr);
	REQUIRE(varDeclare->getClassAssignment() == nullptr);
	REQUIRE(varDeclare->getIdentifier() == "test4");

	AssignmentStatement* assign = varDeclare->getAssignment();
	REQUIRE(assign->getAssign()->getNodeType() == NodeType::Color);

	Color* color = static_cast<Color*>(assign->getAssign());
	REQUIRE(color->getColor() == "#123456");




	REQUIRE(rootNode->getChild(5)->getNodeType() == NodeType::AssignmentStatement);

	assign = static_cast<AssignmentStatement*>(rootNode->getChild(5));
	REQUIRE(assign->getAssign()->getNodeType() == NodeType::Color);

	color = static_cast<Color*>(assign->getAssign());
	REQUIRE(color->getColor() == "#654321");

}