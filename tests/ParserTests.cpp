#include "SourceReader.h"
#include "Lexer.h"
#include "Parser.h"
#include "Token.h"
#include "TokenType.h"
#include "ProgramRootNode.h"
#include "Node.h"
#include <iostream> 

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("IFStatement", "[if]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Parser parser(lexer);

	reader->setSourceString("if (22) {} else {}");

	std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
	std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();
	REQUIRE(firstNode.get()->getNodeType() == NodeType::IfStatement);
	std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(firstNode);
	
	std::shared_ptr<Node> trueBlock = ifStatement.get()->getTrueBlockNode();
	REQUIRE(trueBlock.get()->getNodeType() == NodeType::InstructionsBlock);
	
	std::shared_ptr<Node> elseBlock = ifStatement.get()->getElseBlockNode();
	REQUIRE(elseBlock.get()->getNodeType() == NodeType::InstructionsBlock);
}



TEST_CASE("RepeatStatement", "[repeat]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Parser parser(lexer);

	reader->setSourceString("repeat(-22 + 22 - (22)) {}");

	std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
	std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();
	REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatStatement);

}

TEST_CASE("RepeatTimeStatement", "[repeat]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Parser parser(lexer);

	reader->setSourceString("repeatTime(22) {}");

	std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
	std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();
	REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatTimeStatement);
}


TEST_CASE("Expressions", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Parser parser(lexer);

	SECTION("Simple expression - just digit (zero)")
	{
		reader->setSourceString("repeat(0) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement.get()->getHowManyTime();
		REQUIRE(expression.get()->getTermsSize() == 1);
		
		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 0);
		REQUIRE(!expressionFactor.get()->getNegativeOp());
	}
	
	SECTION("Simple expression - negative digit")
	{
		reader->setSourceString("repeat(-95) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement.get()->getHowManyTime();
		REQUIRE(expression.get()->getTermsSize() == 1);

		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 95);
		REQUIRE(expressionFactor.get()->getNegativeOp());
	}

	SECTION("Simple expression - simple math operation (+, -, *, /) without bracket")
	{
		reader->setSourceString("repeat(-95 + 20 * -20 / -30 - 20) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement.get()->getHowManyTime();
		// A + B - C
		REQUIRE(expression.get()->getTermsSize() == 3);
		REQUIRE(expression.get()->getOperator(0) == TokenType::Plus);
		REQUIRE(expression.get()->getOperator(1) == TokenType::Minus);

		// A Term   -95 
		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 95);
		REQUIRE(expressionFactor.get()->getNegativeOp());



		// B Term  20 * -20 / -30
		expressionTerm = expression.get()->getExpressionTerm(1);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 3);
		REQUIRE(expressionTerm.get()->getOperator(0) == TokenType::Multiply);
		REQUIRE(expressionTerm.get()->getOperator(1) == TokenType::Divide);
		
		expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		// 20 * ...
		REQUIRE(expressionFactor.get()->getIntVal() == 20);
		REQUIRE(!expressionFactor.get()->getNegativeOp());
		
		expressionFactor = expressionTerm.get()->getExpressionFactor(1);
		// ... * -20 / ...
		REQUIRE(expressionFactor.get()->getIntVal() == 20);
		REQUIRE(expressionFactor.get()->getNegativeOp());

		expressionFactor = expressionTerm.get()->getExpressionFactor(2);
		// ... / -30 
		REQUIRE(expressionFactor.get()->getIntVal() == 30);
		REQUIRE(expressionFactor.get()->getNegativeOp());


		// C Term 20
		// Even its  (- 20) its not negation, its substraction
		expressionTerm = expression.get()->getExpressionTerm(2);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

		expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 20);
		REQUIRE(!expressionFactor.get()->getNegativeOp());
	}

	SECTION("Simple expression - brackets")
	{
		reader->setSourceString("repeat(-(2 + -2) * 2) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement.get()->getHowManyTime();
		REQUIRE(expression.get()->getTermsSize() == 1);

		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 2);

		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(1);
		REQUIRE(!expressionFactor.get()->getNegativeOp());
		REQUIRE(expressionFactor.get()->getIntVal() == 2);

		expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getNegativeOp());

		// bracket exprssion (2 + -2)
		std::shared_ptr<Expression> bracketExpression = expressionFactor.get()->getExpression();
		REQUIRE(bracketExpression.get()->getTermsSize() == 2);
		REQUIRE(bracketExpression.get()->getOperator(0) == TokenType::Plus);
		std::shared_ptr<ExpressionTerm> bracketTerm = bracketExpression.get()->getExpressionTerm(0);
		REQUIRE(bracketTerm.get()->getFactorsSize() == 1);
		std::shared_ptr<ExpressionFactor> bracketFactor = bracketTerm.get()->getExpressionFactor(0);
		REQUIRE(!bracketFactor.get()->getNegativeOp());
		REQUIRE(bracketFactor.get()->getIntVal() == 2);
		bracketTerm = bracketExpression.get()->getExpressionTerm(1);
		REQUIRE(bracketTerm.get()->getFactorsSize() == 1);
		bracketFactor = bracketTerm.get()->getExpressionFactor(0);
		REQUIRE(bracketFactor.get()->getNegativeOp());
		REQUIRE(bracketFactor.get()->getIntVal() == 2);
	}
}

//TEST_CASE("getNextToken", "[nextToken]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer* lexer = new Lexer(reader);
//	Parser parser(lexer);
//
//	reader->setSourceString("if else return");
//	Token temp;
//	
//	REQUIRE(!parser.hasBufferedToken());
//	temp = parser.getNextToken();
//	REQUIRE(temp.type == TokenType::If);
//	REQUIRE(!parser.hasBufferedToken());
//	temp = parser.getNextToken();
//	REQUIRE(temp.type == TokenType::Else);
//	REQUIRE(!parser.hasBufferedToken());
//	temp = parser.getNextToken();
//	REQUIRE(temp.type == TokenType::Return);
//	REQUIRE(!parser.hasBufferedToken());
//}
//
//TEST_CASE("peekToken", "[peek]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer* lexer = new Lexer(reader);
//	Parser parser(lexer);
//
//	reader->setSourceString("if else return");
//	Token temp;
//
//	REQUIRE(!parser.hasBufferedToken());
//	temp = parser.peekToken();
//	REQUIRE(temp.type == TokenType::If);
//	REQUIRE(parser.hasBufferedToken());
//	temp = parser.peekToken();
//	REQUIRE(temp.type == TokenType::If);
//	REQUIRE(parser.hasBufferedToken());
//	temp = parser.getNextToken();
//	REQUIRE(temp.type == TokenType::If);
//	REQUIRE(!parser.hasBufferedToken());
//	temp = parser.getNextToken();
//	REQUIRE(temp.type == TokenType::Else);
//	REQUIRE(!parser.hasBufferedToken());
//	temp = parser.peekToken();
//	REQUIRE(temp.type == TokenType::Return);
//	REQUIRE(parser.hasBufferedToken());
//	temp = parser.getNextToken();
//	REQUIRE(temp.type == TokenType::Return);
//	REQUIRE(!parser.hasBufferedToken());
//}