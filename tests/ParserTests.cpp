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

	reader->setSourceString("repeat(-(-22)) {}");

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


//TEST_CASE("Expressions", "[parser]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer* lexer = new Lexer(reader);
//	Parser parser(lexer);
//
//	SECTION("Simple expression - just digit")
//	{
//		reader->setSourceString("repeat(-22) {}");
//		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
//		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();
//
//		REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatStatement);
//
//		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
//		std::shared_ptr<Expression> expression = repeatStatement.get()->getHowManyTime();
//	}
//	
//
//}

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