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

	// SKIP_LINE TEST
	reader.setSourceString("Test\nString");
	reader.skipLine();
	REQUIRE(reader.getCharacter() == 'S');
}


TEST_CASE("File as source", "[fileReader]")
{
	SourceReader reader;
	reader.setSourceFile("fileReader_TestFile.txt");
	REQUIRE(reader.peek() == 'T');
	REQUIRE(reader.getCharNumber() == 0);
	REQUIRE(reader.getLineNumber() == 1);
	REQUIRE(reader.isEof() == false);

	// NEW LINE TEST
	reader.getCharacter();
	reader.getCharacter(); 
	reader.getCharacter();
	REQUIRE(reader.getCharacter() == 't');
	REQUIRE(reader.getCharacter() == 'S');
	REQUIRE(reader.getCharNumber() == 1);
	REQUIRE(reader.getLineNumber() == 2);
	REQUIRE(reader.isEof() == false);
}