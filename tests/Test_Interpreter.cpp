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

	SECTION("XXX")
	{
		reader.setSourceString("function test() {}");

		Interpreter interpreter(parser.parse());
		interpreter.run();
	}

	SECTION("YYY")
	{
		reader.setSourceString("if(false) {} else {}");

		Interpreter interpreter(parser.parse());
		interpreter.run();
	}
}