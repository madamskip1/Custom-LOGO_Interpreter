#include "SourceReader.h"
#include "Lexer.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


TEST_CASE("EmptySource Token", "[token]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	reader->setSourceString("");

	Token token = lexer.nextToken();
	REQUIRE(token.type == TokenType::EndOfFile);
}

TEST_CASE("Digit Token", "[digit]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	reader->setSourceString("12345");
	
	token = lexer.nextToken();

	REQUIRE(token.type == TokenType::Digit);
	REQUIRE(std::get<int>(token.value) == 12345);

	reader->setSourceString("         6789");

	token = lexer.nextToken();

	REQUIRE(token.type == TokenType::Digit);
	REQUIRE(std::get<int>(token.value) == 6789);




	reader->setSourceString("000000.1000");

	token = lexer.nextToken();

	REQUIRE(token.type == TokenType::Digit);
	REQUIRE(std::get<int>(token.value) == 0);

	lexer.nextToken();
	 
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Digit);
	REQUIRE(std::get<int>(token.value) == 1000);
}

TEST_CASE("String Token", "[stringToken]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	reader->setSourceString("\"Test String\"");

	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::StringVal);
	REQUIRE(std::get<std::string>(token.value) == "Test String");


	reader->setSourceString("\"\"");

	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::StringVal);
	REQUIRE(std::get<std::string>(token.value) == "");



	reader->setSourceString("\"");

	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::INVALID);

	reader->setSourceString(" 2323\"TestString\"");
	lexer.nextToken();
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::StringVal);
	REQUIRE(std::get<std::string>(token.value) == "TestString");
}

TEST_CASE("Keywords", "[keywords]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	reader->setSourceString("repeat function return");
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Repeat);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Function);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Return);
}

TEST_CASE("Identifier", "[id]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	reader->setSourceString("id1");
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	REQUIRE(std::get<std::string>(token.value) == "id1");

	reader->setSourceString("i_d_1_");

	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	REQUIRE(std::get<std::string>(token.value) == "i_d_1_");
}

TEST_CASE("MathOperators", "[mathOP]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	reader->setSourceString("+1/-2*");
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Plus);
	token = lexer.nextToken();
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Divide);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Minus);
	token = lexer.nextToken();
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Multiply);

	reader->setSourceString("+1//");
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Plus);
	token = lexer.nextToken();
	token = lexer.nextToken();
	REQUIRE(token.type != TokenType::Divide);
}

TEST_CASE("ConditionOperators", "[conditionOP]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	reader->setSourceString("2 && 2 || 3");
	token = lexer.nextToken();
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::And);
	token = lexer.nextToken();
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Or);


	reader->setSourceString("2 &&& 2 | 3");
	token = lexer.nextToken();
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::And);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::INVALID);
	token = lexer.nextToken();
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::INVALID);
}

TEST_CASE("Relation Operator", "[relationOP]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	reader->setSourceString("2 == 2");
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Equal);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Digit);

	reader->setSourceString("===== !!= 1");
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Equal);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Equal);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::AssignOperator);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::NotOperator);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::NotEqual); 

	reader->setSourceString("< <= >= >");
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Less);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::LessOrEqual);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::GreaterOrEqual);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Greater);

}

TEST_CASE("Brackets", "[brackets]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	reader->setSourceString("() {}");
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::CurlyBracketOpen);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::CurlyBracketClose);
}

TEST_CASE("Symbols", "[symbols]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	reader->setSourceString(". , ;");
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Comma);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Semicolon);
}

TEST_CASE("Comments", "[comments]")
{
	SourceReader* reader = new SourceReader();
	Lexer lexer(reader);
	Token token;

	reader->setSourceString("/// qwewqe \n true");

	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Comment);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::True);
}



TEST_CASE("Sample Code", "[code]")
{
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
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Repeat);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::CurlyBracketOpen);

	// 2nd line
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Semicolon);

	// 3rd line
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::CurlyBracketClose);

	// 4th line
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::StringVal);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Semicolon);


	// 5th line
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::AssignOperator);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::True);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Semicolon);

	// 6th line
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::If);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Less);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::CurlyBracketOpen);

	// 7th line
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Plus);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Semicolon);

	// 8th line
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::CurlyBracketClose);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Else);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::CurlyBracketOpen);


	// 9th line
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Dot);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Minus);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Semicolon);


	// 10th line
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::CurlyBracketClose);

	// 11th line
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Turtle);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Semicolon);

	// 12th line
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Point);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Identifier);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketOpen);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Comma);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Digit);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::RoundBracketClose);
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::Semicolon);

	// EOF
	token = lexer.nextToken();
	REQUIRE(token.type == TokenType::EndOfFile);
}