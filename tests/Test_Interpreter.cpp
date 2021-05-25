#include "SourceReader.h"
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"

#pragma warning(push, 0)        
#include "catch.hpp"
#pragma warning(pop)

TEST_CASE("simple interpreter", "[interpreter]")
{
	SourceReader reader;
	Lexer lexer(reader);
	Logger logger;
	Parser parser(lexer, logger);

	//SECTION("XXX")
	//{
	//	reader.setSourceString("function test() {}");

	//	Interpreter interpreter(parser.parse());
	//	interpreter.run();
	//}

	//SECTION("YYY")
	//{
	//	reader.setSourceString("if(false) {} else {}");

	//	Interpreter interpreter(parser.parse());
	//	interpreter.run();
	//}

	//SECTION("repeat")
	//{
	//	reader.setSourceString("repeat(5) {}");

	//	Interpreter interpreter(parser.parse());
	//	interpreter.run();
	//}

	SECTION("int declare assign")
	{
		reader.setSourceString("Integer test = 4; test = 5;");

		Interpreter interpreter(parser.parse());
		interpreter.run();
	}

	SECTION("Boolean declare assign")
	{
		reader.setSourceString("Boolean test1 = true; test1 = false;");

		Interpreter interpreter(parser.parse());
		interpreter.run();
	}

	SECTION("Var declare in scope")
	{
		reader.setSourceString("if (true) { Integer test; }");

		Interpreter interpreter(parser.parse());
		interpreter.run();
	}

	SECTION("execute function")
	{
		reader.setSourceString("function test() {} test();");

		Interpreter interpreter(parser.parse());
		interpreter.run();
	}
}