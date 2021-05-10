//#include "SourceReader.h"
//
//#define CATCH_CONFIG_MAIN
//#include "catch.hpp"
//
//TEST_CASE("String as source", "[stringReader]")
//{
//	SourceReader reader;
//	reader.setSourceString("Test\nString");
//
//	// getCharacter TEST
//	REQUIRE(reader.getCharacter() == 'T');
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.isEof() == false);
//
//	// getNextCharacter TEST
//	REQUIRE(reader.getNextCharacter() == 'e');
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.isEof() == false);	
//	REQUIRE(reader.getCharacter() == 'e');
//	REQUIRE(reader.getCharNumber() == 2);
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.isEof() == false);
//}
//
//
//TEST_CASE("File as source", "[fileReader]")
//{
//	SourceReader reader;
//	reader.setSourceFile("fileReader_TestFile.txt");
//
//	// getCharacter TEST
//	REQUIRE(reader.getCharacter() == 'T');
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.isEof() == false);
//
//	// getNextCharacter TEST
//	REQUIRE(reader.getNextCharacter() == 'e');
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.isEof() == false);
//	REQUIRE(reader.getCharacter() == 'e');
//	REQUIRE(reader.getCharNumber() == 2);
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.isEof() == false);
//}
//
//TEST_CASE("End of line", "[endOfLine]")
//{
//	SourceReader reader;
//
//	// Simple EndOfLine
//	reader.setSourceString("Te\nSt");
//	REQUIRE(reader.getCharNumber() == 0);
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.getCharacter() == 'T');
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(reader.getLineNumber() == 1);
//	reader.getCharacter();
//	REQUIRE(reader.getCharNumber() == 2);
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.getCharacter() == 'S');
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(reader.getLineNumber() == 2);
//
//
//	// Multi EndOfLine in a row with getCharacter
//	std::string string = R"(Line1
//Line2
//
//
//
//    Line6 
//
//Line8)";
//
//	reader.setSourceString(string);
//	REQUIRE(reader.getCharacter() == 'L');
//	reader.getCharacter();
//	reader.getCharacter();
//	reader.getCharacter();
//	REQUIRE(reader.getCharacter() == '1');
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.getCharacter() == 'L');
//	REQUIRE(reader.getLineNumber() == 2);
//	reader.getCharacter();
//	reader.getCharacter();
//	reader.getCharacter();
//	reader.getCharacter();
//	REQUIRE(reader.getCharacter() == ' ');
//	REQUIRE(reader.getLineNumber() == 6);
//	REQUIRE(reader.getCharNumber() == 1);
//	reader.getCharacter();
//	reader.getCharacter();
//	reader.getCharacter();
//	REQUIRE(reader.getCharacter() == 'L');
//	REQUIRE(reader.getLineNumber() == 6);
//	REQUIRE(reader.getCharNumber() == 5);
//}
//
//TEST_CASE("EndOfFile", "[eof]")
//{
//	SourceReader reader;
//
//	// Simple EndOfFile
//	reader.setSourceString("Te\nSt");
//
//	REQUIRE(reader.getCharacter() == 'T');
//
//	reader.getCharacter();
//	REQUIRE(reader.getCharacter() == 'S');
//
//	reader.getCharacter();
//	REQUIRE(!reader.isEof());
//	reader.getNextCharacter();
//	REQUIRE(reader.getCharacter() == '\0');
//	REQUIRE(reader.isEof());
//
//
//
//
//	// empty string
//	reader.setSourceString("");
//
//	REQUIRE(!reader.isEof());
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.getCharNumber() == 0);
//
//	REQUIRE(reader.getCharacter() == '\0');
//	REQUIRE(reader.isEof());
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.getCharNumber() == 1);
//
//
//	// getChar after EOF
//	reader.setSourceString("x");
//
//	REQUIRE(reader.getCharacter() == 'x');
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(!reader.isEof());
//
//	REQUIRE(reader.getCharacter() == '\0');
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.getCharNumber() == 2);
//	REQUIRE(reader.isEof());
//
//	reader.getCharacter();
//	reader.getCharacter();
//	reader.getCharacter();
//	reader.getCharacter();
//	REQUIRE(reader.getCharacter() == '\0');
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.getCharNumber() == 2);
//	REQUIRE(reader.isEof());
//
//	// getChar then eof after few empty lines
//	std::string string = R"(T
//
//
//)";
//	reader.setSourceString(string);
//	REQUIRE(reader.getCharacter() == 'T');
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(!reader.isEof());
//	char test = reader.getCharacter();
//	REQUIRE(test == '\0');
//	REQUIRE(reader.getLineNumber() == 4);
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(reader.isEof());
//}
//
//TEST_CASE("Skip lines", "[skipLine]")
//{
//	SourceReader reader;
//
//	// simple skip line
//	reader.setSourceString("Test \n   String");
//	REQUIRE(reader.getCharacter() == 'T');
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.getCharNumber() == 1);
//	reader.skipLine();
//	REQUIRE(reader.getLineNumber() == 2);
//	REQUIRE(reader.getCharNumber() == 0);
//	REQUIRE(reader.getCharacter() == ' ');
//	REQUIRE(reader.getLineNumber() == 2);
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(!reader.isEof());
//	reader.skipLine();
//	REQUIRE(reader.getLineNumber() == 2);
//	REQUIRE(reader.getCharNumber() == 10);
//	REQUIRE(reader.isEof());
//
//	// Skip line with endofFile in next line
//	std::string string = R"(Test
//)";
//	reader.setSourceString(string);
//	REQUIRE(reader.getCharacter() == 'T');
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(!reader.isEof());
//	reader.skipLine();
//	REQUIRE(reader.getLineNumber() == 2);
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(reader.isEof());
//
//	// skip line and endOfFile after few empty lines
//	string = R"(Test
//
//
//
//
//)";
//	reader.setSourceString(string);
//	REQUIRE(reader.getCharacter() == 'T');
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(!reader.isEof());
//	reader.skipLine();
//	REQUIRE(reader.getLineNumber() == 6);
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(reader.isEof());
//
//
//	// Multi empty lines
//	string = R"(Line1
//Line2
//
//
//
//Line6 
//
//     Line8
//
//Line10
//)";
//
//	reader.setSourceString(string);
//	REQUIRE(reader.getCharacter() == 'L');
//	REQUIRE(reader.getLineNumber() == 1);
//	REQUIRE(reader.getCharNumber() == 1);
//	reader.skipLine();
//	REQUIRE(reader.getLineNumber() == 2);
//	REQUIRE(reader.getCharNumber() == 0);
//	REQUIRE(reader.getCharacter() == 'L');
//	REQUIRE(reader.getLineNumber() == 2);
//	REQUIRE(reader.getCharNumber() == 1);
//	reader.skipLine();
//	REQUIRE(reader.getLineNumber() == 6);
//	REQUIRE(reader.getCharNumber() == 0);
//	REQUIRE(reader.getCharacter() == 'L');
//	REQUIRE(reader.getLineNumber() == 6);
//	REQUIRE(reader.getCharNumber() == 1);
//	reader.skipLine();
//	REQUIRE(reader.getLineNumber() == 8);
//	REQUIRE(reader.getCharNumber() == 0);
//	REQUIRE(reader.getCharacter() == ' ');
//	reader.skipLine();
//	REQUIRE(reader.getLineNumber() == 10);
//	REQUIRE(reader.getCharNumber() == 0);
//	REQUIRE(reader.getCharacter() == 'L');
//	REQUIRE(reader.getCharNumber() == 1);
//	reader.skipLine();
//	REQUIRE(reader.getLineNumber() == 11);
//	REQUIRE(reader.getCharNumber() == 1);
//	REQUIRE(reader.isEof());
//}