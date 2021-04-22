#include "SourceReader.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("String as source", "[stringReader]")
{
	SourceReader reader;
	reader.setSourceString("Test\nString");

	// PEEK TEST
	REQUIRE(reader.peek() == 'T');
	REQUIRE(reader.getCharNumber() == 0);
	REQUIRE(reader.getLineNumber() == 1);
	REQUIRE(reader.isEof() == false);
	
	// GET CHAR TEST
	REQUIRE(reader.getCharacter() == 'T');
	REQUIRE(reader.getCharNumber() == 1);
	REQUIRE(reader.getLineNumber() == 1);
	REQUIRE(reader.isEof() == false);
	

	// NEW LINE TEST
	reader.getCharacter();
	reader.getCharacter();
	REQUIRE(reader.getCharacter() == 't');
	REQUIRE(reader.getCharacter() == 'S');
	REQUIRE(reader.getCharNumber() == 1);
	REQUIRE(reader.getLineNumber() == 2);
	REQUIRE(reader.isEof() == false);

	// END OF FILE
	reader.getCharacter();
	reader.getCharacter();
	reader.getCharacter();
	reader.getCharacter();
	REQUIRE(reader.getCharacter() == 'g');
	REQUIRE(!reader.isEof());
	reader.getCharacter();
	REQUIRE(reader.isEof());


	reader.setSourceString("12345");
	REQUIRE(reader.getCharacter() == '1');
	REQUIRE(reader.peek() == '2');
	REQUIRE(reader.getCharacter() == '2');

	reader.setSourceString("Test\nString");
	reader.skipLine();
	REQUIRE(reader.getCharacter() == 'S');
}

TEST_CASE("empty string", "[empty_stringReader]")
{
	SourceReader reader;
	reader.setSourceString("");
	reader.peek();
	REQUIRE(reader.isEof());
}