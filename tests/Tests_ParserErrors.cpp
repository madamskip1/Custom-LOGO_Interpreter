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
    SourceReader reader;
    Lexer lexer(reader);
    Parser parser(lexer);
    Logger* logger = Logger::getInstance();

    SECTION("no error")
    {
        reader.setSourceString("if (true) { Integer test = 2; } else { calcFunc(20); Turtle turtle;}");

        logger->clearLogs();
        parser.parse();
        REQUIRE(logger->getLogsSize() == 0);
        REQUIRE(logger->hasAnyError() == false);
    }

    SECTION("Missing  round bracket open")
    {
        reader.setSourceString("if )");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketOpen);
        }
    }
	
    SECTION("Missing round bracket close")
    {
        reader.setSourceString("if (true");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
        }
    }

    SECTION("No condition")
    {
        reader.setSourceString("if () {}");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::BadExpression);
        }
    }
}

TEST_CASE("Repeat Statement", "[parserBad]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Parser parser(lexer);
    Logger* logger = Logger::getInstance();

    SECTION("no error")
    {
        reader.setSourceString("repeat(20 - 10) { Integer test = 2; }");

        logger->clearLogs();
        parser.parse();
        REQUIRE(logger->getLogsSize() == 0);
        REQUIRE(logger->hasAnyError() == false);
    }

    SECTION("Missing  round bracket open")
    {
        reader.setSourceString("repeat 20)");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketOpen);
        }
    }

    SECTION("Missing round bracket close")
    {
        reader.setSourceString("repeat(20 ");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
        }
    }

    SECTION("No expression")
    {
        reader.setSourceString("repeat() {} ");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::BadExpression);
        }
    }
}

TEST_CASE("Repeat time Statement", "[parserBad]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Parser parser(lexer);
    Logger* logger = Logger::getInstance();

    SECTION("no error")
    {
        reader.setSourceString("repeatTime(20 - 10, 20 * 2) { Integer test = 2; }");

        logger->clearLogs();
        parser.parse();
        REQUIRE(logger->getLogsSize() == 0);
        REQUIRE(logger->hasAnyError() == false);
    }

    SECTION("Missing  round bracket open")
    {
        reader.setSourceString("repeatTime 20)");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketOpen);
        }
    }

    SECTION("Missing round bracket close")
    {
        reader.setSourceString("repeatTime (20 ");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
        }
    }

    SECTION("Missing expression")
    {
        reader.setSourceString("repeatTime () {} ");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::BadExpression);
        }
    }

    SECTION("Missing second expression")
    {
        reader.setSourceString("repeatTime (20, ) {} ");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::BadExpression);
        }
    }

    SECTION("Missing comma")
    {
        reader.setSourceString("repeatTime (20 20) {} ");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
        }
    }
}

TEST_CASE("call Func", "[parserBad]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Parser parser(lexer);
    Logger* logger = Logger::getInstance();

    SECTION("no error")
    {
        reader.setSourceString("callFunc1(); if (callFunc2()) {}");

        logger->clearLogs();
        parser.parse();
        REQUIRE(logger->getLogsSize() == 0);
        REQUIRE(logger->hasAnyError() == false);
    }

    SECTION("missing round bracket close")
    {
        reader.setSourceString("callFunc1(;");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::BadExpression);
        }
    }

    SECTION("missing semicolon")
    {
        reader.setSourceString("callFunc1()");

        logger->clearLogs();
        parser.parse();
        REQUIRE(logger->getLogsSize() == 1);
        REQUIRE(logger->hasAnyError() == false);
        REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingSemicolon);
    }

    SECTION("missing parameter")
    {
        reader.setSourceString("callFunc1(20, )");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::BadExpression);
        }
    }

    SECTION("missing comma")
    {
        reader.setSourceString("callFunc1(20 20)");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
        }
    }
}

TEST_CASE("var declaration", "[parserBad]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Parser parser(lexer);
    Logger* logger = Logger::getInstance();

    SECTION("no error")
    {
        reader.setSourceString("Boolean test1; Integer test2 = 20;");

        logger->clearLogs();
        parser.parse();
        REQUIRE(logger->getLogsSize() == 0);
    }

    SECTION("missing identifier")
    {
        reader.setSourceString("Boolean test1; Integer ;");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingIdentifierOrFunctionKeyword);
        }
    }

    SECTION("missing semicolown")
    {
        reader.setSourceString("Integer test1");

        logger->clearLogs();
        parser.parse();
        REQUIRE(logger->getLogsSize() == 1);
        REQUIRE(logger->hasAnyError() == false);
        REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingSemicolon);
    }

    SECTION("with assignment, missing semicolon")
    {
        reader.setSourceString("Integer test1 = 20");

        logger->clearLogs();
        parser.parse();
        REQUIRE(logger->getLogsSize() == 1);
        REQUIRE(logger->hasAnyError() == false);
        REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingSemicolon);
    }

    SECTION("missing assignment after operator")
    {
        reader.setSourceString("Integer test1 = ");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::BadExpression);
        }
    }

    SECTION("var declare class missing parameter")
    {
        reader.setSourceString("Turtle turtle();");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::BadExpression);
        }
    }

    SECTION("var declare class missing parameter after comma")
    {
        reader.setSourceString("Turtle turtle(20, );");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::BadExpression);
        }
    }

    SECTION("var declare class missingcomma")
    {
        reader.setSourceString("Turtle turtle(20 20);");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingCurlyBracketClose);
        }
    }
}

TEST_CASE("def func", "[parserBad]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Parser parser(lexer);
    Logger* logger = Logger::getInstance();

    SECTION("no error")
    {
        reader.setSourceString("function test() {}");

        logger->clearLogs();
        parser.parse();
        REQUIRE(logger->getLogsSize() == 0);
    }

    SECTION("missing identifier")
    {
        reader.setSourceString("function () {}");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingIdentifier);
        }
    }

    SECTION("bad parameter syntax")
    {
        reader.setSourceString("function test(test) {}");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::BadSyntaxParameter);
        }
    }

    SECTION("missing parameter identifier")
    {
        reader.setSourceString("function test(Turtle) {}");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingIdentifier);
        }
    }

    SECTION("missing parameter after comma")
    {
        reader.setSourceString("function test(Turtle test1, ) {}");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::BadSyntaxParameter);
        }
    }

    SECTION("missing comma")
    {
        reader.setSourceString("function test(Turtle test1 Point 2) {}");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingRoundBracketClose);
        }
    }
}

TEST_CASE("assignment", "[parserBad]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Parser parser(lexer);
    Logger* logger = Logger::getInstance();

    SECTION("no error")
    {
        reader.setSourceString("test = 200;");

        logger->clearLogs();
        parser.parse();
        REQUIRE(logger->getLogsSize() == 0);
        REQUIRE(logger->hasAnyError() == false);
    }

    SECTION("missing assing after operator")
    {
        reader.setSourceString("test = ;");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::BadExpression);
        }
    }

    SECTION("missing operator")
    {
        reader.setSourceString("test;");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::NotEndOfFile);
        }
    }
}

TEST_CASE("Instruction Block", "[parserBad]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Parser parser(lexer);
    Logger* logger = Logger::getInstance();

    SECTION("no error")
    {
        reader.setSourceString("if(true) {}");

        logger->clearLogs();
        parser.parse();
        REQUIRE(logger->getLogsSize() == 0);
        REQUIRE(logger->hasAnyError() == false);
    }

    SECTION("missing curly bracket open")
    {
        reader.setSourceString("if(true) }");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingCurlyBracketOpen);
        }
    }

    SECTION("missing curly bracket close")
    {
        reader.setSourceString("if(true) { ");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::MissingCurlyBracketClose);
        }
    }

    SECTION("def func in instructions block")
    {
        reader.setSourceString("if(true) { function test() {} } ");

        logger->clearLogs();
        try {
            parser.parse();
        }
        catch (const std::exception& e)
        {
            REQUIRE(logger->getLogsSize() == 1);
            REQUIRE(logger->hasAnyError() == true);
            REQUIRE(logger->getLog(0)->getLogType() == LogType::CantDefFuncInBlock);
        }
    }
}


TEST_CASE("return statement", "[parserBad]")
{
    SourceReader reader;
    Lexer lexer(reader);
    Parser parser(lexer);
    Logger* logger = Logger::getInstance();

    SECTION("no error")
    {
        reader.setSourceString("function test() { return true; }");

        logger->clearLogs();
        parser.parse();
        REQUIRE(logger->getLogsSize() == 0);
        REQUIRE(logger->hasAnyError() == false);
    }
}
