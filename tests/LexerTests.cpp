#include "SourceReader.h"
#include "Lexer.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


TEST_CASE("EmptySource Token", "[token]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);

	// No token - end of file
	reader->setSourceString("");
	Token token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::EndOfFile);
}

TEST_CASE("Invalid Token", "[invalid]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);

	reader->setSourceString("_");
	Token token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::INVALID);
}

TEST_CASE("Digit Token", "[digit]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;
	
	// Simple digit
	reader->setSourceString("12345");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);
	REQUIRE(std::get<int>(token.value) == 12345);

	// digit with zeroes before
	reader->setSourceString("         00000000678900");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);
	REQUIRE(std::get<int>(token.value) == 678900);

	// zeroes dot digit
	reader->setSourceString("000000.1000");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);
	REQUIRE(std::get<int>(token.value) == 0);
	lexer.getNextToken();
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);
	REQUIRE(std::get<int>(token.value) == 1000);
}

TEST_CASE("String Token", "[stringToken]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	// Simple string
	reader->setSourceString("\"Test String\"");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::StringVal);
	REQUIRE(std::get<std::string>(token.value) == "Test String");

	// Empty string
	reader->setSourceString("\"\"");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::StringVal);
	REQUIRE(std::get<std::string>(token.value) == "");

	// Not terminated string
	reader->setSourceString("\""); 
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::INVALID);

	// digit then string
	reader->setSourceString(" 2323\"TestString\"");
	lexer.getNextToken();
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::StringVal);
	REQUIRE(std::get<std::string>(token.value) == "TestString");
}

TEST_CASE("Keywords", "[keywords]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	// simple three keywords
	reader->setSourceString("repeat function return");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Repeat);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Function);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Return);
}

TEST_CASE("Identifier", "[id]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	// simple identifier
	reader->setSourceString("id1");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	REQUIRE(std::get<std::string>(token.value) == "id1");

	// identifier with Underscore symbol
	reader->setSourceString("i_d_1_");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	REQUIRE(std::get<std::string>(token.value) == "i_d_1_");
}

TEST_CASE("MathOperators", "[mathOP]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	// All math operators
	reader->setSourceString("+1/-2*");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Plus);
	token = lexer.getNextToken();
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Divide);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Minus);
	token = lexer.getNextToken();
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Multiply);

	// Double divide symbol in a row => Comment
	reader->setSourceString("+1//");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Plus);
	token = lexer.getNextToken();
	token = lexer.getNextToken();
	REQUIRE(token.type != TokenType::Divide);
	REQUIRE(token.type == TokenType::Comment);
}

TEST_CASE("ConditionOperators", "[conditionOP]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	// AND and OR operators
	reader->setSourceString("2 && 2 || 3");
	token = lexer.getNextToken();
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::And);
	token = lexer.getNextToken();
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Or);

	// Incorrect operators 
	reader->setSourceString("2 &&& 2 | 3");
	token = lexer.getNextToken();
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::And);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::INVALID);
	token = lexer.getNextToken();
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::INVALID);
}

TEST_CASE("Relation Operator", "[relationOP]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	// Equal operator
	reader->setSourceString("2 == 2");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Equal);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);

	// combination of equal, assign, not and notEqual operators
	reader->setSourceString("===== !!= 1");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Equal);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Equal);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::AssignOperator);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::NotOperator);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::NotEqual); 

	// Relation operators less, greater etc
	reader->setSourceString("< <= >= >");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Less);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::LessOrEqual);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::GreaterOrEqual);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Greater);
}

TEST_CASE("Brackets", "[brackets]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	// Round and curly brackets 
	reader->setSourceString("() {}");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::CurlyBracketOpen);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::CurlyBracketClose);
}

TEST_CASE("Symbols", "[symbols]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	// dot, comma and semicolon symbols
	reader->setSourceString(". , ;");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Comma);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Semicolon);
}

TEST_CASE("Comments", "[comments]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	// comments 
	reader->setSourceString("/// qwewqe \n true");
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Comment);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::True);
}



TEST_CASE("Sample Code", "[code]")
{
	// A test case that checks the correct analysis of sample code that can be used in the program.
	// Includes combinations of different designs

	// Code contains: repeat keyword, digits, identifier, dot/comma/semicolon symbol, string, bracket, relation operators, mathOperators, data types 
	std::string code = 
R"(repeat(10) {
	zolw1.go(10);
}
zolw1.brush.color.set("#000000");
zolw2.hidden = true;

if (x < 20) {
	zolw.go(2 + 2);
} else {
	zolw.go(x - 20);
}
Turtle zolwik;
Point point(1, 0);
)";

	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	reader->setSourceString(code);

	// 1st Line
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Repeat);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::CurlyBracketOpen);

	// 2nd line
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Semicolon);

	// 3rd line
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::CurlyBracketClose);

	// 4th line
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::StringVal);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Semicolon);


	// 5th line
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::AssignOperator);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::True);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Semicolon);

	// 6th line
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::If);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Less);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::CurlyBracketOpen);

	// 7th line
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Plus);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Semicolon);

	// 8th line
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::CurlyBracketClose);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Else);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::CurlyBracketOpen);


	// 9th line
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Minus);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Semicolon);


	// 10th line
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::CurlyBracketClose);

	// 11th line
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Turtle);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Semicolon);

	// 12th line
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Point);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Comma);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::Semicolon);

	// EOF
	token = lexer.getNextToken();
	REQUIRE(token.type == TokenType::EndOfFile);
}