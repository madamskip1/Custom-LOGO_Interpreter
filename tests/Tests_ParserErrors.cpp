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
    SourceReader reader;;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);

    SECTION("no error")
    {
        reader.setSourceString("if (true) { Integer test = 2; } else { calcFunc(20); Turtle turtle;}");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 0);
        REQUIRE(logger.hasAnyError() == false);
    }

    SECTION("Missing  round bracket open")
    {
        reader.setSourceString("if )");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingRoundBracketOpen);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }
	
    SECTION("Missing round bracket close")
    {
        reader.setSourceString("if (true");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 1);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
    }

    SECTION("No condition")
    {
        reader.setSourceString("if () {}");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 3);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::BadExpression);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::BadCondition);
        REQUIRE(logger.getLog(2)->getLogType() == LogType::NotEndOfFile);
    }
}

TEST_CASE("Repeat Statement", "[parserBad]")
{
    SourceReader reader;;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);

    SECTION("no error")
    {
        reader.setSourceString("repeat(20 - 10) { Integer test = 2; }");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 0);
        REQUIRE(logger.hasAnyError() == false);
    }

    SECTION("Missing  round bracket open")
    {
        reader.setSourceString("repeat 20)");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingRoundBracketOpen);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("Missing round bracket close")
    {
        reader.setSourceString("repeat(20 ");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 1);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
    }

    SECTION("No expression")
    {
        reader.setSourceString("repeat() {} ");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 3);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::BadExpression);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::BadCondition);
        REQUIRE(logger.getLog(2)->getLogType() == LogType::NotEndOfFile);
    }
}

TEST_CASE("Repeat time Statement", "[parserBad]")
{
    SourceReader reader;;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);

    SECTION("no error")
    {
        reader.setSourceString("repeatTime(20 - 10, 20 * 2) { Integer test = 2; }");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 0);
        REQUIRE(logger.hasAnyError() == false);
    }

    SECTION("Missing  round bracket open")
    {
        reader.setSourceString("repeatTime 20)");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingRoundBracketOpen);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("Missing round bracket close")
    {
        reader.setSourceString("repeatTime (20 ");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 1);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
    }

    SECTION("Missing expression")
    {
        reader.setSourceString("repeatTime () {} ");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 3);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::BadExpression);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::BadCondition);
        REQUIRE(logger.getLog(2)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("Missing second expression")
    {
        reader.setSourceString("repeatTime (20, ) {} ");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 3);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::BadExpression);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::BadCondition);
        REQUIRE(logger.getLog(2)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("Missing comma")
    {
        reader.setSourceString("repeatTime (20 20) {} ");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }
}

TEST_CASE("call Func", "[parserBad]")
{
    SourceReader reader;;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);

    SECTION("no error")
    {
        reader.setSourceString("callFunc1(); if (callFunc2()) {}");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 0);
        REQUIRE(logger.hasAnyError() == false);
    }

    SECTION("missing round bracket close")
    {
        reader.setSourceString("callFunc1(;");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 4);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::BadExpression);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::BadCondition);
        REQUIRE(logger.getLog(2)->getLogType() == LogType::MissingParameter);
        REQUIRE(logger.getLog(3)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("missing semicolon")
    {
        reader.setSourceString("callFunc1()");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 1);
        REQUIRE(logger.hasAnyError() == false);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingSemicolon);
    }

    SECTION("missing parameter")
    {
        reader.setSourceString("callFunc1(20, )");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 4);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::BadExpression);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::BadCondition);
        REQUIRE(logger.getLog(2)->getLogType() == LogType::MissingParameter);
        REQUIRE(logger.getLog(3)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("missing comma")
    {
        reader.setSourceString("callFunc1(20 20)");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }
}

TEST_CASE("var declaration", "[parserBad]")
{
    SourceReader reader;;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);

    SECTION("no error")
    {
        reader.setSourceString("Boolean test1; Integer test2 = 20;");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 0);
    }

    SECTION("missing identifier")
    {
        reader.setSourceString("Boolean test1; Integer ;");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingIdentifierOrFunctionKeyword);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("missing semicolown")
    {
        reader.setSourceString("Integer test1");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 1);
        REQUIRE(logger.hasAnyError() == false);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingSemicolon);
    }

    SECTION("with assignment, missing semicolon")
    {
        reader.setSourceString("Integer test1 = 20");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 1);
        REQUIRE(logger.hasAnyError() == false);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingSemicolon);
    }

    SECTION("missing assignment after operator")
    {
        reader.setSourceString("Integer test1 = ");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 3);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::BadExpression);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::BadCondition);
        REQUIRE(logger.getLog(2)->getLogType() == LogType::UnknownAssignable);
    }

    SECTION("var declare class missing parameter")
    {
        reader.setSourceString("Turtle turtle();");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::BadExpression);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("var declare class missing parameter after comma")
    {
        reader.setSourceString("Turtle turtle(20, );");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 3);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::BadExpression);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::BadCondition);
        REQUIRE(logger.getLog(2)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("var declare class missingcomma")
    {
        reader.setSourceString("Turtle turtle(20 20);");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingCurlyBracketClose);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }
}

TEST_CASE("def func", "[parserBad]")
{
    SourceReader reader;;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);

    SECTION("no error")
    {
        reader.setSourceString("function test() {}");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 0);
    }

    SECTION("missing identifier")
    {
        reader.setSourceString("function () {}");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingIdentifier);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("bad parameter syntax")
    {
        reader.setSourceString("function test(test) {}");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::BadSyntaxParameter);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("missing parameter identifier")
    {
        reader.setSourceString("function test(Turtle) {}");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingIdentifier);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("missing parameter after comma")
    {
        reader.setSourceString("function test(Turtle test1, ) {}");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::BadSyntaxParameter);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("missing comma")
    {
        reader.setSourceString("function test(Turtle test1 Point 2) {}");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.hasAnyError() == true);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }
}

TEST_CASE("assignment", "[parserBad]")
{
    SourceReader reader;;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);

    SECTION("no error")
    {
        reader.setSourceString("test = 200;");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 0);
    }

    SECTION("missing assing after operator")
    {
        reader.setSourceString("test = ;");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 4);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::BadExpression);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::BadCondition);
        REQUIRE(logger.getLog(2)->getLogType() == LogType::UnknownAssignable);
        REQUIRE(logger.getLog(3)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("missing operator")
    {
        reader.setSourceString("test;");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 1);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::NotEndOfFile);
    }
}

TEST_CASE("Instruction Block", "[parserBad]")
{
    SourceReader reader;;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);

    SECTION("no error")
    {
        reader.setSourceString("if(true) {}");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 0);
    }

    SECTION("missing curly bracket open")
    {
        reader.setSourceString("if(true) }");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingCurlyBracketOpen);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }

    SECTION("missing curly bracket close")
    {
        reader.setSourceString("if(true) { ");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 1);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::MissingCurlyBracketClose);
    }

    SECTION("def func in instructions block")
    {
        reader.setSourceString("if(true) { function test() {} } ");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 2);
        REQUIRE(logger.getLog(0)->getLogType() == LogType::CantDefFuncInBlock);
        REQUIRE(logger.getLog(1)->getLogType() == LogType::NotEndOfFile);
    }
}


TEST_CASE("return statement", "[parserBad]")
{
    SourceReader reader;;
    Lexer lexer(reader);
    Logger logger;
    Parser parser(lexer, logger);

    SECTION("no error")
    {
        reader.setSourceString("function test() { return true; }");

        parser.parse();
        REQUIRE(logger.getLogsSize() == 0);
    }
}
