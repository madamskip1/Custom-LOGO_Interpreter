#include <iostream>
#include "SourceReader.h"
#include "Lexer.h"
#include "Parser.h"
#include "Logger.h"

#pragma warning(push, 0)        
#include "catch.hpp"
#pragma warning(pop)

TEST_CASE("If Statement", "[parserBad]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("no error")
	{
		reader->setSourceString("if (true) { Integer test = 2; } else { calcFunc(20); Turtle turtle;}");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 0);
	}

	SECTION("Missing  round bracket open")
	{
		reader->setSourceString("if )");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 1);
		REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketOpen);
	}
	
	SECTION("Missing round bracket close")
	{
		reader->setSourceString("if (");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 2);
		REQUIRE(logger->getLog(0)->getLogType() == LogType::BadExpression);
		REQUIRE(logger->getLog(1)->getLogType() == LogType::MissingRoundBracketClose);
	}
}

TEST_CASE("Repeat Statement", "[parserBad]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("no error")
	{
		reader->setSourceString("repeat(20 - 10) { Integer test = 2; }");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 0);
	}

	SECTION("Missing  round bracket open")
	{
		reader->setSourceString("repeat 20)");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 1);
		REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketOpen);
	}

	SECTION("Missing round bracket close")
	{
		reader->setSourceString("repeat(20 ");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 1);
		REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
	}
}


TEST_CASE("Repeat time Statement", "[parserBad]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("no error")
	{
		reader->setSourceString("repeatTime(20 - 10, 20 * 2) { Integer test = 2; }");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 0);
	}

	SECTION("Missing  round bracket open")
	{
		reader->setSourceString("repeatTime 20)");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 1);
		REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketOpen);
	}

	SECTION("Missing round bracket close")
	{
		reader->setSourceString("repeatTime (20 ");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 1);
		REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
	}
}

TEST_CASE("call Func", "[parserBad]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("no error")
	{
		reader->setSourceString("callFunc1(); if (callFunc2()) {}");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 0);
	}

	SECTION("missing round bracket close")
	{
		reader->setSourceString("callFunc1(;");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 2);
		REQUIRE(logger->getLog(0)->getLogType() == LogType::BadExpression);
		REQUIRE(logger->getLog(1)->getLogType() == LogType::MissingRoundBracketClose);
	}

	SECTION("missing semicolon")
	{
		reader->setSourceString("callFunc1()");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 1);
		REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingSemicolon);
	}
}

TEST_CASE("var declaration", "[parserBad]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("no error")
	{
		reader->setSourceString("Boolean test1; Integer test2 = 20;");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 0);
	}

	SECTION("missing identifier")
	{
		reader->setSourceString("Boolean test1; Integer ;");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 1);
		REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingIdentifierOrFunctionKeyword);
	}

	SECTION("missing semicolown")
	{
		reader->setSourceString("Integer test1");

		parser.parse();
		REQUIRE(logger->getLogsSize() == 1);
		REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingSemicolon);
	}
}

TEST_CASE("Bad expressions", "[parserBad]")
{
	SECTION("missing round bracket close")
	{

	}


}