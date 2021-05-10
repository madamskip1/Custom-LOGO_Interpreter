//#include "SourceReader.h"
//#include "Lexer.h"
//
//#define CATCH_CONFIG_MAIN
//#include "catch.hpp"
//
//
//TEST_CASE("EmptySource Token", "[token]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer lexer(reader);
//
//	// No token - end of file
//	reader->setSourceString("");
//	Token token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::EndOfFile);
//}
//
//TEST_CASE("Invalid Token", "[invalid]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer lexer(reader);
//
//	reader->setSourceString("_");
//	Token token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::INVALID);
//}
//
//TEST_CASE("Digit Token", "[digit]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer lexer(reader);
//	Token token;
//	
//	// Simple digit
//	reader->setSourceString("12345");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Digit);
//	REQUIRE(token.getIntValue() == 12345);
//
//	// digit with zeroes before
//	reader->setSourceString("         000000006789 false");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::BadDigitZeros);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::False);
//
//	// zeroes dot digit
//	reader->setSourceString("000000.1000");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::BadDigitZeros);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Dot);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Digit);
//	REQUIRE(token.getIntValue() == 1000);
//
//	// digit too long
//	reader->setSourceString("12345678900000000000 true");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::BadDigitTooLong);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::True);
//}
//
//TEST_CASE("ColorValue Token", "[colorValueToken]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer lexer(reader);
//	Token token;
//
//	// Simple string
//	reader->setSourceString("\"#123456\"");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::ColorValue);
//	REQUIRE(token.getStringValue() == "#123456");
//
//	// Not terminated string
//	reader->setSourceString("\""); 
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::ColorValNotTerminated);
//
//	// Hash missing
//	reader->setSourceString("\"1234567\"");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::ColorValMissHash);
//
//	// Bad syntax 
//	reader->setSourceString("\"#1234GG\"");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::ColorValBadSyntax);
//
//	// hash missing and bad syntax
//	reader->setSourceString("\"11234GG\"");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::ColorValMissHash);
//
//	// To short
//	reader->setSourceString("\"#12345\"");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::ColorValTooShort);
//
//	// To long
//	reader->setSourceString("\"#1234567\"");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::ColorValTooLong);
//
//	// Empty string
//	reader->setSourceString("\"\"");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::ColorValTooShort);
//}
//
//TEST_CASE("Keywords", "[keywords]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer lexer(reader);
//	Token token;
//
//	// simple three keywords and indetifier with almost same name as keyword
//	reader->setSourceString("repeat function return Return");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Repeat);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Function);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Return);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	REQUIRE(token.getStringValue() == "Return");
//
//	// true/false
//	reader->setSourceString("true false");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::True);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::False);
//}
//
//TEST_CASE("Identifier", "[id]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer lexer(reader);
//	Token token;
//
//	// simple identifier
//	reader->setSourceString("id1");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	REQUIRE(token.getStringValue() == "id1");
//
//	// identifier with Underscore symbol
//	reader->setSourceString("i_d_1_");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	REQUIRE(token.getStringValue() == "i_d_1_");
//}
//
//TEST_CASE("MathOperators", "[mathOP]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer lexer(reader);
//	Token token;
//
//	// All math operators
//	reader->setSourceString("+1/-2***");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Plus);
//	token = lexer.getNextToken();
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Divide);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Minus);
//	token = lexer.getNextToken();
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Multiply);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Multiply);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Multiply);
//
//	// Double divide symbol in a row => Comment
//	reader->setSourceString("+1+//");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Plus);
//	token = lexer.getNextToken();
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Plus);
//	token = lexer.getNextToken();
//	REQUIRE(token.type != TokenType::Divide);
//	REQUIRE(token.type == TokenType::EndOfFile);
//}
//
//TEST_CASE("ConditionOperators", "[conditionOP]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer lexer(reader);
//	Token token;
//
//	// AND and OR operators
//	reader->setSourceString("2 && 2 || 3");
//	token = lexer.getNextToken();
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::And);
//	token = lexer.getNextToken();
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Or);
//
//	// Incorrect operators 
//	reader->setSourceString("2 &&& 2 | 3");
//	token = lexer.getNextToken();
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::And);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::AndOperatorMissSecond);
//	token = lexer.getNextToken();
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::OrOperatorMissSecond);
//}
//
//TEST_CASE("Relation Operator", "[relationOP]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer lexer(reader);
//	Token token;
//
//	// Equal operator
//	reader->setSourceString("2 == 2");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Digit);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Equal);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Digit);
//
//	// combination of equal, assign, not and notEqual operators
//	reader->setSourceString("===== !!= 1");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Equal);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Equal);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::AssignOperator);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::NotOperator);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::NotEqual); 
//
//	// Relation operators less, greater etc
//	reader->setSourceString("< <= >= >");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Less);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::LessOrEqual);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::GreaterOrEqual);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Greater);
//}
//
//TEST_CASE("Brackets", "[brackets]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer lexer(reader);
//	Token token;
//
//	// Round and curly brackets 
//	reader->setSourceString("() {}");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketOpen);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketClose);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::CurlyBracketOpen);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::CurlyBracketClose);
//}
//
//TEST_CASE("Symbols", "[symbols]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer lexer(reader);
//	Token token;
//
//	// dot, comma and semicolon symbols
//	reader->setSourceString(". ,;");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Dot);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Comma);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Semicolon);
//}
//
//TEST_CASE("Comments", "[comments]")
//{
//	SourceReader* reader = new SourceReader();
//	Lexer lexer(reader);
//	Token token;
//
//	// single comment
//	reader->setSourceString("/// qwewqe \n true");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::True);
//
//
//	// multi comments in a row
//	std::string string = R"( //XXX
//
// // 2xxxx  / / / / //// // / / / / / //
//			// 3xxxx
//
///////////////////////////////////////
//					///////////////////
//
//// 4 xxx
//123456789;
//
////
////
////)";
//
//	reader->setSourceString(string);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Digit);
//	REQUIRE(token.getIntValue() == 123456789);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Semicolon);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::EndOfFile);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::EndOfFile);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::EndOfFile);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::EndOfFile);
//}
//
//
//
//TEST_CASE("Sample Code", "[code]")
//{
//	// A test case that checks the correct analysis of sample code that can be used in the program.
//	// Includes combinations of different designs
//
//	// Code contains: repeat keyword, digits, identifier, dot/comma/semicolon symbol, string, bracket, relation operators, mathOperators, data types 
//	std::string code = 
//R"(repeat(10) {
//	zolw1.go(10);
//}
//zolw1.brush.color.set("#000000"); // XXXXXX COMMENT
//zolw2.hidden = true;
//
//if (x < 20) {								// XXXXXX COMMENT
//	zolw.go(2 + 2);
//} else {
//	zolw.go(x - 20);
//}
//
//Turtle zolwik;
//
// Point point(1, 0);
//)";
//
//	SourceReader* reader = new SourceReader();
//	Lexer lexer(reader);
//	Token token;
//
//	reader->setSourceString(code);
//
//	// 1st Line
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Repeat);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketOpen);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Digit);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketClose);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::CurlyBracketOpen);
//
//	// 2nd line
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Dot);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketOpen);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Digit);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketClose);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Semicolon);
//
//	// 3rd line
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::CurlyBracketClose);
//
//	// 4th line
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Dot);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Dot);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Dot);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketOpen);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::ColorValue);
//	REQUIRE(token.getStringValue() == "#000000");
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketClose);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Semicolon);
//
//
//	// 5th line
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Dot);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::AssignOperator);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::True);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Semicolon);
//
//	// 6th line
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::If);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketOpen);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Less);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Digit);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketClose);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::CurlyBracketOpen);
//
//	// 7th line
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Dot);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketOpen);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Digit);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Plus);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Digit);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketClose);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Semicolon);
//
//	// 8th line
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::CurlyBracketClose);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Else);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::CurlyBracketOpen);
//
//
//	// 9th line
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Dot);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketOpen);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Minus);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Digit);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketClose);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Semicolon);
//
//
//	// 10th line
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::CurlyBracketClose);
//
//	// 11th line
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Turtle);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Semicolon);
//
//	// 12th line
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Point);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Identifier);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketOpen);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Digit);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Comma);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Digit);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::RoundBracketClose);
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::Semicolon);
//
//	// EOF
//	token = lexer.getNextToken();
//	REQUIRE(token.type == TokenType::EndOfFile);
//}