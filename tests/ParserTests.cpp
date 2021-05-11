#include "SourceReader.h"
#include "Lexer.h"
#include "Parser.h"
#include "Token.h"
#include "TokenType.h"
#include "ProgramRootNode.h"
#include "Node.h"
#include "CallFuncStatement.h"
#include <iostream> 

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("IFStatement", "[if]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Parser parser(lexer);

	reader->setSourceString("if (false) {} else {}");

	std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
	std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();
	REQUIRE(firstNode.get()->getNodeType() == NodeType::IfStatement);
	std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(firstNode);

	std::shared_ptr<Condition> condition = ifStatement.get()->getCondition();
	REQUIRE(condition.get()->getNodeType() == NodeType::Condition);
	REQUIRE(condition.get()->getAndConditionSize() == 1);
	
	std::shared_ptr<AndCondition> andCondition = condition.get()->getAndCondition();
	REQUIRE(andCondition.get()->getNodeType() == NodeType::AndCondition);
	REQUIRE(andCondition.get()->getRelationConditionSize() == 1);

	std::shared_ptr<RelationCondition> relationCondition = andCondition.get()->getRelationCondition();
	REQUIRE(relationCondition.get()->getNodeType() == NodeType::RelationCondition);
	REQUIRE(relationCondition.get()->isBooleanWord());
	REQUIRE(!relationCondition.get()->getBooleanWord());
	REQUIRE(!relationCondition.get()->getNotOperator());
	REQUIRE(relationCondition.get()->getCondition() == nullptr);
	REQUIRE(relationCondition.get()->getExpression() == nullptr);

	
	std::shared_ptr<Node> trueBlock = ifStatement.get()->getTrueBlockNode();
	REQUIRE(trueBlock.get()->getNodeType() == NodeType::InstructionsBlock);
	
	std::shared_ptr<Node> elseBlock = ifStatement.get()->getElseBlockNode();
	REQUIRE(elseBlock.get()->getNodeType() == NodeType::InstructionsBlock);

}



TEST_CASE("RepeatStatement", "[repeat]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Parser parser(lexer);

	reader->setSourceString("repeat(50) {}");

	std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
	std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();
	REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatStatement);

	std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
	
	std::shared_ptr<Node> block = repeatStatement.get()->getInstructuionsBlock();
	REQUIRE(block.get()->getNodeType() == NodeType::InstructionsBlock);
	
	std::shared_ptr<Expression> expression = repeatStatement.get()->getHowManyTime();
	REQUIRE(expression.get()->getTermsSize() == 1);

	std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
	REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

	std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
	REQUIRE(expressionFactor.get()->getIntVal() == 50);
	REQUIRE(!expressionFactor.get()->getNegativeOp());
}

TEST_CASE("RepeatTimeStatement", "[repeat]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Parser parser(lexer);

	reader->setSourceString("repeatTime(22) {}");

	std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
	std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();
	REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatTimeStatement);

	std::shared_ptr<RepeatTimeStatement> repeatTimeStatement = std::static_pointer_cast<RepeatTimeStatement>(firstNode);
	
	std::shared_ptr<Node> block = repeatTimeStatement.get()->getInstructuionsBlock();
	REQUIRE(block.get()->getNodeType() == NodeType::InstructionsBlock);
	
	std::shared_ptr<Expression> expression = repeatTimeStatement.get()->getPeriod();
	REQUIRE(expression.get()->getTermsSize() == 1);

	std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
	REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

	std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
	REQUIRE(expressionFactor.get()->getIntVal() == 22);
	REQUIRE(!expressionFactor.get()->getNegativeOp());

	expression = repeatTimeStatement.get()->getHowManyTime();
	REQUIRE(expression == nullptr);
}


TEST_CASE("Expressions", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Parser parser(lexer);

	SECTION("Simple expression - just digit (zero)")
	{
		reader->setSourceString("repeat(0) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement.get()->getHowManyTime();
		REQUIRE(expression.get()->getTermsSize() == 1);
		
		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 0);
		REQUIRE(!expressionFactor.get()->getNegativeOp());
	}
	
	SECTION("Simple expression - negative digit")
	{
		reader->setSourceString("repeat(-95) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement.get()->getHowManyTime();
		REQUIRE(expression.get()->getTermsSize() == 1);

		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 95);
		REQUIRE(expressionFactor.get()->getNegativeOp());
	}

	SECTION("math operation (+, -, *, /) without bracket")
	{
		reader->setSourceString("repeat(-95 + 20 * -20 / -30 - 20) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement.get()->getHowManyTime();
		// A + B - C
		REQUIRE(expression.get()->getTermsSize() == 3);
		REQUIRE(expression.get()->getOperator(0) == TokenType::Plus);
		REQUIRE(expression.get()->getOperator(1) == TokenType::Minus);

		// A Term   -95 
		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 95);
		REQUIRE(expressionFactor.get()->getNegativeOp());



		// B Term  20 * -20 / -30
		expressionTerm = expression.get()->getExpressionTerm(1);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 3);
		REQUIRE(expressionTerm.get()->getOperator(0) == TokenType::Multiply);
		REQUIRE(expressionTerm.get()->getOperator(1) == TokenType::Divide);
		
		expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		// 20 * ...
		REQUIRE(expressionFactor.get()->getIntVal() == 20);
		REQUIRE(!expressionFactor.get()->getNegativeOp());
		
		expressionFactor = expressionTerm.get()->getExpressionFactor(1);
		// ... * -20 / ...
		REQUIRE(expressionFactor.get()->getIntVal() == 20);
		REQUIRE(expressionFactor.get()->getNegativeOp());

		expressionFactor = expressionTerm.get()->getExpressionFactor(2);
		// ... / -30 
		REQUIRE(expressionFactor.get()->getIntVal() == 30);
		REQUIRE(expressionFactor.get()->getNegativeOp());


		// C Term 20
		// Even its  (- 20) its not negation, its substraction
		expressionTerm = expression.get()->getExpressionTerm(2);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

		expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 20);
		REQUIRE(!expressionFactor.get()->getNegativeOp());
	}

	SECTION("expression with brackets")
	{
		reader->setSourceString("repeat(-(2 + -2) * 2) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement.get()->getHowManyTime();
		REQUIRE(expression.get()->getTermsSize() == 1);

		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 2);

		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(1);
		REQUIRE(!expressionFactor.get()->getNegativeOp());
		REQUIRE(expressionFactor.get()->getIntVal() == 2);

		expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getNegativeOp());

		// bracket exprssion (2 + -2)
		std::shared_ptr<Expression> bracketExpression = expressionFactor.get()->getExpression();
		REQUIRE(bracketExpression.get()->getTermsSize() == 2);
		REQUIRE(bracketExpression.get()->getOperator(0) == TokenType::Plus);
		std::shared_ptr<ExpressionTerm> bracketTerm = bracketExpression.get()->getExpressionTerm(0);
		REQUIRE(bracketTerm.get()->getFactorsSize() == 1);
		std::shared_ptr<ExpressionFactor> bracketFactor = bracketTerm.get()->getExpressionFactor(0);
		REQUIRE(!bracketFactor.get()->getNegativeOp());
		REQUIRE(bracketFactor.get()->getIntVal() == 2);
		bracketTerm = bracketExpression.get()->getExpressionTerm(1);
		REQUIRE(bracketTerm.get()->getFactorsSize() == 1);
		bracketFactor = bracketTerm.get()->getExpressionFactor(0);
		REQUIRE(bracketFactor.get()->getNegativeOp());
		REQUIRE(bracketFactor.get()->getIntVal() == 2);
	}

	SECTION("expression fun call")
	{
		reader->setSourceString("repeat(callFunc(20)) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement.get()->getHowManyTime();
		REQUIRE(expression.get()->getTermsSize() == 1);

		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 0);
		REQUIRE(!expressionFactor.get()->getNegativeOp());
		REQUIRE(!expressionFactor.get()->getExpression());
		std::shared_ptr<CallFuncStatement> callFunc = expressionFactor.get()->getCallFunc();
		REQUIRE(callFunc.get()->getNodeType() == NodeType::CallFuncStatement);
		REQUIRE(callFunc.get()->getArgumentsSize() == 1);
		REQUIRE(callFunc.get()->getIdentifiersSize() == 1);
	}
}



TEST_CASE("Conditions", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Parser parser(lexer);

	SECTION("simple relation Operator")
	{
		reader->setSourceString("if (2 < 3) {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();
		REQUIRE(firstNode.get()->getNodeType() == NodeType::IfStatement);
		std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(firstNode);

		std::shared_ptr<Condition> condition = ifStatement.get()->getCondition();
		REQUIRE(condition.get()->getNodeType() == NodeType::Condition);
		REQUIRE(condition.get()->getAndConditionSize() == 1);

		std::shared_ptr<AndCondition> andCondition = condition.get()->getAndCondition();
		REQUIRE(andCondition.get()->getNodeType() == NodeType::AndCondition);
		REQUIRE(andCondition.get()->getRelationConditionSize() == 1);

		std::shared_ptr<RelationCondition> relationCondition = andCondition.get()->getRelationCondition();
		REQUIRE(relationCondition.get()->getNodeType() == NodeType::RelationCondition);
		REQUIRE(!relationCondition.get()->isBooleanWord());
		REQUIRE(!relationCondition.get()->getNotOperator());
		REQUIRE(relationCondition.get()->getCondition() == nullptr);
		REQUIRE(relationCondition.get()->hasSecondExpression());
		REQUIRE(relationCondition.get()->getRelationOperator() == TokenType::Less);
	}

	SECTION("AND and OR operators, not ")
	{
		reader->setSourceString("if (!true && false || true) {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();
		REQUIRE(firstNode.get()->getNodeType() == NodeType::IfStatement);
		std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(firstNode);

		std::shared_ptr<Condition> condition = ifStatement.get()->getCondition();
		REQUIRE(condition.get()->getNodeType() == NodeType::Condition);
		REQUIRE(condition.get()->getAndConditionSize() == 2);

		std::shared_ptr<AndCondition> andCondition = condition.get()->getAndCondition();
		REQUIRE(andCondition.get()->getNodeType() == NodeType::AndCondition);
		REQUIRE(andCondition.get()->getRelationConditionSize() == 2);

		std::shared_ptr<RelationCondition> relationCondition = andCondition.get()->getRelationCondition();
		REQUIRE(relationCondition.get()->getNodeType() == NodeType::RelationCondition);
		REQUIRE(relationCondition.get()->isBooleanWord());
		REQUIRE(relationCondition.get()->getBooleanWord());
		REQUIRE(relationCondition.get()->getNotOperator());
		REQUIRE(relationCondition.get()->getCondition() == nullptr);
		REQUIRE(!relationCondition.get()->hasSecondExpression());
		REQUIRE(relationCondition.get()->getExpression() == nullptr);

		relationCondition = andCondition.get()->getRelationCondition(1);
		REQUIRE(relationCondition.get()->getNodeType() == NodeType::RelationCondition);
		REQUIRE(relationCondition.get()->isBooleanWord());
		REQUIRE(!relationCondition.get()->getBooleanWord());
		REQUIRE(!relationCondition.get()->getNotOperator());
		REQUIRE(relationCondition.get()->getCondition() == nullptr);
		REQUIRE(!relationCondition.get()->hasSecondExpression());
		REQUIRE(relationCondition.get()->getExpression() == nullptr);


		andCondition = condition.get()->getAndCondition(1);
		REQUIRE(andCondition.get()->getNodeType() == NodeType::AndCondition);
		REQUIRE(andCondition.get()->getRelationConditionSize() == 1);

		relationCondition = andCondition.get()->getRelationCondition();
		REQUIRE(relationCondition.get()->getNodeType() == NodeType::RelationCondition);
		REQUIRE(relationCondition.get()->isBooleanWord());
		REQUIRE(relationCondition.get()->getBooleanWord());
		REQUIRE(!relationCondition.get()->getNotOperator());
		REQUIRE(relationCondition.get()->getCondition() == nullptr);
		REQUIRE(!relationCondition.get()->hasSecondExpression());
		REQUIRE(relationCondition.get()->getExpression() == nullptr);

	}

	SECTION("Brackets")
	{
		reader->setSourceString("if (!(callFunc() || 2 == 2) && true) {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();
		REQUIRE(firstNode.get()->getNodeType() == NodeType::IfStatement);
		std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(firstNode);

		std::shared_ptr<Condition> condition = ifStatement.get()->getCondition();
		REQUIRE(condition.get()->getNodeType() == NodeType::Condition);
		REQUIRE(condition.get()->getAndConditionSize() == 1);

		std::shared_ptr<AndCondition> andCondition = condition.get()->getAndCondition();
		REQUIRE(andCondition.get()->getNodeType() == NodeType::AndCondition);
		REQUIRE(andCondition.get()->getRelationConditionSize() == 2);

		std::shared_ptr<RelationCondition> relationCondition = andCondition.get()->getRelationCondition(1);
		REQUIRE(relationCondition.get()->getNodeType() == NodeType::RelationCondition);
		REQUIRE(relationCondition.get()->isBooleanWord());
		REQUIRE(relationCondition.get()->getBooleanWord());
		REQUIRE(!relationCondition.get()->getNotOperator());
		REQUIRE(relationCondition.get()->getCondition() == nullptr);
		REQUIRE(!relationCondition.get()->hasSecondExpression());
		REQUIRE(relationCondition.get()->getExpression() == nullptr);


		relationCondition = andCondition.get()->getRelationCondition();
		REQUIRE(relationCondition.get()->getNodeType() == NodeType::RelationCondition);
		REQUIRE(!relationCondition.get()->isBooleanWord());
		REQUIRE(relationCondition.get()->getNotOperator());
		REQUIRE(relationCondition.get()->getCondition() != nullptr);
		REQUIRE(!relationCondition.get()->hasSecondExpression());
		REQUIRE(relationCondition.get()->getExpression() == nullptr);

		condition = relationCondition.get()->getCondition();
		REQUIRE(condition.get()->getNodeType() == NodeType::Condition);
		REQUIRE(condition.get()->getAndConditionSize() == 2);

		andCondition = condition.get()->getAndCondition();
		REQUIRE(andCondition.get()->getNodeType() == NodeType::AndCondition);
		REQUIRE(andCondition.get()->getRelationConditionSize() == 1);

		relationCondition = andCondition.get()->getRelationCondition();
		REQUIRE(relationCondition.get()->getNodeType() == NodeType::RelationCondition);
		REQUIRE(!relationCondition.get()->isBooleanWord());
		REQUIRE(!relationCondition.get()->getNotOperator());
		REQUIRE(relationCondition.get()->getCondition() == nullptr);
		REQUIRE(!relationCondition.get()->hasSecondExpression());
		REQUIRE(relationCondition.get()->getExpression() != nullptr);

		andCondition = condition.get()->getAndCondition(1);
		REQUIRE(andCondition.get()->getNodeType() == NodeType::AndCondition);
		REQUIRE(andCondition.get()->getRelationConditionSize() == 1);

		relationCondition = andCondition.get()->getRelationCondition();
		REQUIRE(relationCondition.get()->getNodeType() == NodeType::RelationCondition);
		REQUIRE(!relationCondition.get()->isBooleanWord());
		REQUIRE(!relationCondition.get()->getNotOperator());
		REQUIRE(relationCondition.get()->getCondition() == nullptr);
		REQUIRE(relationCondition.get()->hasSecondExpression());
		REQUIRE(relationCondition.get()->getExpression() != nullptr);
		REQUIRE(relationCondition.get()->getRelationOperator() == TokenType::Equal);
	}
}



TEST_CASE("def function", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Parser parser(lexer);

	SECTION("simple function, without parameters, without return type")
	{
		reader->setSourceString("function test() {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::DefFuncStatement);

		std::shared_ptr<DefFuncStatement> defFunction = std::static_pointer_cast<DefFuncStatement>(firstNode);
		REQUIRE(defFunction.get()->getName() == "test");
		REQUIRE(defFunction.get()->getParametersSize() == 0);
		REQUIRE(!defFunction.get()->hasReturnType());
	}

	SECTION("function with one parameters, without return type")
	{
		reader->setSourceString("function test2(Turtle zolw) {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::DefFuncStatement);

		std::shared_ptr<DefFuncStatement> defFunction = std::static_pointer_cast<DefFuncStatement>(firstNode);
		REQUIRE(defFunction.get()->getName() == "test2");
		REQUIRE(defFunction.get()->getParametersSize() == 1);
		REQUIRE(!defFunction.get()->hasReturnType());

		std::shared_ptr<Parameter> parameter = defFunction.get()->getParameter(0);
		REQUIRE(parameter.get()->getType() == TokenType::Turtle);
		REQUIRE(parameter.get()->getName() == "zolw");
	}

	SECTION("function with multi parameters, witout return type")
	{
		reader->setSourceString("function test3(Turtle zolw, ColorVar color, Integer int) {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::DefFuncStatement);

		std::shared_ptr<DefFuncStatement> defFunction = std::static_pointer_cast<DefFuncStatement>(firstNode);
		REQUIRE(defFunction.get()->getName() == "test3");
		REQUIRE(defFunction.get()->getParametersSize() == 3);
		REQUIRE(!defFunction.get()->hasReturnType());

		std::shared_ptr<Parameter> parameter = defFunction.get()->getParameter(0);
		REQUIRE(parameter.get()->getType() == TokenType::Turtle);
		REQUIRE(parameter.get()->getName() == "zolw");
		
		parameter = defFunction.get()->getParameter(1);
		REQUIRE(parameter.get()->getType() == TokenType::ColorVar);
		REQUIRE(parameter.get()->getName() == "color");

		parameter = defFunction.get()->getParameter(2);
		REQUIRE(parameter.get()->getType() == TokenType::Integer);
		REQUIRE(parameter.get()->getName() == "int");
	}

	SECTION("function without parameters, with return type")
	{
		reader->setSourceString("Turtle function test4() {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::DefFuncStatement);

		std::shared_ptr<DefFuncStatement> defFunction = std::static_pointer_cast<DefFuncStatement>(firstNode);
		REQUIRE(defFunction.get()->getName() == "test4");
		REQUIRE(defFunction.get()->getParametersSize() == 0);
		REQUIRE(defFunction.get()->hasReturnType());
		REQUIRE(defFunction.get()->getReturnType() == TokenType::Turtle);
	}

	SECTION("function witn parameters, with return type")
	{
		reader->setSourceString("Integer function test5(Point point, Integer int) {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::DefFuncStatement);

		std::shared_ptr<DefFuncStatement> defFunction = std::static_pointer_cast<DefFuncStatement>(firstNode);
		REQUIRE(defFunction.get()->getName() == "test5");
		REQUIRE(defFunction.get()->getParametersSize() == 2);
		REQUIRE(defFunction.get()->hasReturnType());
		REQUIRE(defFunction.get()->getReturnType() == TokenType::Integer);

		std::shared_ptr<Parameter> parameter = defFunction.get()->getParameter(0);
		REQUIRE(parameter.get()->getType() == TokenType::Point);
		REQUIRE(parameter.get()->getName() == "point");

		parameter = defFunction.get()->getParameter(1);
		REQUIRE(parameter.get()->getType() == TokenType::Integer);
		REQUIRE(parameter.get()->getName() == "int");
	}
}

TEST_CASE("call function", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Parser parser(lexer);

	SECTION("simple call function")
	{
		reader->setSourceString("test();");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::CallFuncStatement);

		std::shared_ptr<CallFuncStatement> callFunc = std::static_pointer_cast<CallFuncStatement>(firstNode);
		REQUIRE(callFunc.get()->getArgumentsSize() == 0);
		REQUIRE(callFunc.get()->getIdentifiersSize() == 1);
		REQUIRE(callFunc.get()->getIdentifier(0) == "test");
	}

	SECTION("call function with one argument, int")
	{
		reader->setSourceString("test2(153);");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::CallFuncStatement);

		std::shared_ptr<CallFuncStatement> callFunc = std::static_pointer_cast<CallFuncStatement>(firstNode);
		REQUIRE(callFunc.get()->getArgumentsSize() == 1);
		REQUIRE(callFunc.get()->getIdentifiersSize() == 1);
		REQUIRE(callFunc.get()->getIdentifier(0) == "test2");


		std::shared_ptr<Expression> expression = callFunc.get()->getArgument(0);
		REQUIRE(expression.get()->getTermsSize() == 1);

		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 153);
		REQUIRE(!expressionFactor.get()->getNegativeOp());
	}

	SECTION("call function with more arguments")
	{
		reader->setSourceString("test3(160, -20, 10);");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::CallFuncStatement);

		std::shared_ptr<CallFuncStatement> callFunc = std::static_pointer_cast<CallFuncStatement>(firstNode);
		REQUIRE(callFunc.get()->getArgumentsSize() == 3);
		REQUIRE(callFunc.get()->getIdentifiersSize() == 1);
		REQUIRE(callFunc.get()->getIdentifier(0) == "test3");


		std::shared_ptr<Expression> expression = callFunc.get()->getArgument(0);
		REQUIRE(expression.get()->getTermsSize() == 1);
		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);
		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 160);
		REQUIRE(!expressionFactor.get()->getNegativeOp());

		expression = callFunc.get()->getArgument(1);
		REQUIRE(expression.get()->getTermsSize() == 1);
		expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);
		expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 20);
		REQUIRE(expressionFactor.get()->getNegativeOp());

		expression = callFunc.get()->getArgument(2);
		REQUIRE(expression.get()->getTermsSize() == 1);
		expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);
		expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 10);
		REQUIRE(!expressionFactor.get()->getNegativeOp());
	}

	SECTION("call function multi level ID")
	{
		reader->setSourceString("test4.test44.test444();");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::CallFuncStatement);

		std::shared_ptr<CallFuncStatement> callFunc = std::static_pointer_cast<CallFuncStatement>(firstNode);
		REQUIRE(callFunc.get()->getArgumentsSize() == 0);
		REQUIRE(callFunc.get()->getIdentifiersSize() == 3);
		REQUIRE(callFunc.get()->getIdentifier(0) == "test4");
		REQUIRE(callFunc.get()->getIdentifier(1) == "test44");
		REQUIRE(callFunc.get()->getIdentifier(2) == "test444");
	}

	SECTION("call function multi level ID and multi args")
	{
		reader->setSourceString("test5.test55.test555(1, -2, 3);");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::CallFuncStatement);

		std::shared_ptr<CallFuncStatement> callFunc = std::static_pointer_cast<CallFuncStatement>(firstNode);
		REQUIRE(callFunc.get()->getArgumentsSize() == 3);
		REQUIRE(callFunc.get()->getIdentifiersSize() == 3);
		REQUIRE(callFunc.get()->getIdentifier(0) == "test5");
		REQUIRE(callFunc.get()->getIdentifier(1) == "test55");
		REQUIRE(callFunc.get()->getIdentifier(2) == "test555");

		std::shared_ptr<Expression> expression = callFunc.get()->getArgument(0);
		REQUIRE(expression.get()->getTermsSize() == 1);
		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);
		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 1);
		REQUIRE(!expressionFactor.get()->getNegativeOp());

		expression = callFunc.get()->getArgument(1);
		REQUIRE(expression.get()->getTermsSize() == 1);
		expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);
		expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 2);
		REQUIRE(expressionFactor.get()->getNegativeOp());

		expression = callFunc.get()->getArgument(2);
		REQUIRE(expression.get()->getTermsSize() == 1);
		expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);
		expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 3);
		REQUIRE(!expressionFactor.get()->getNegativeOp());
	
	}

}


TEST_CASE("assign", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Parser parser(lexer);

	SECTION("simple assign")
	{
		reader->setSourceString("test = -20");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::AssignStatement);

		std::shared_ptr<AssignStatement> assign = std::static_pointer_cast<AssignStatement>(firstNode);
		REQUIRE(assign.get()->getIdentifier() == "test");

		std::shared_ptr<Expression> expression = assign.get()->getExpression();
		REQUIRE(expression != nullptr);
		REQUIRE(expression.get()->getTermsSize() == 1);

		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(expressionFactor.get()->getIntVal() == 20);
		REQUIRE(expressionFactor.get()->getNegativeOp());
	}

	SECTION("multi level id assign and calc Func")
	{
		reader->setSourceString("test2.test22.test222 = calcFunc()");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::AssignStatement);

		std::shared_ptr<AssignStatement> assign = std::static_pointer_cast<AssignStatement>(firstNode);
		REQUIRE(assign.get()->getIdentifier() == "test2");
		REQUIRE(assign.get()->getIdentifier(1) == "test22");
		REQUIRE(assign.get()->getIdentifier(2) == "test222");

		std::shared_ptr<Expression> expression = assign.get()->getExpression();
		REQUIRE(expression != nullptr);
		REQUIRE(expression.get()->getTermsSize() == 1);

		std::shared_ptr<ExpressionTerm> expressionTerm = expression.get()->getExpressionTerm(0);
		REQUIRE(expressionTerm.get()->getFactorsSize() == 1);

		std::shared_ptr<ExpressionFactor> expressionFactor = expressionTerm.get()->getExpressionFactor(0);
		REQUIRE(!expressionFactor.get()->getNegativeOp());
		REQUIRE(expressionFactor.get()->getCallFunc() != nullptr);
	}
}


TEST_CASE("Var declartion", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Parser parser(lexer);

	SECTION("simple def var")
	{
		reader->setSourceString("Integer test;");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::DeclareVarStatement);

		std::shared_ptr<DeclareVarStatement> varDeclare = std::static_pointer_cast<DeclareVarStatement>(firstNode);
		REQUIRE(varDeclare.get()->getIdentifier() == "test");
		REQUIRE(varDeclare.get()->getAssignStatemnt() == nullptr);
		REQUIRE(varDeclare.get()->getType() == TokenType::Integer);
	}


	SECTION("def var with assign")
	{
		reader->setSourceString("Boolean test2 = !true;");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode.get()->getNextInstruction();

		REQUIRE(firstNode.get()->getNodeType() == NodeType::DeclareVarStatement);

		std::shared_ptr<DeclareVarStatement> varDeclare = std::static_pointer_cast<DeclareVarStatement>(firstNode);
		REQUIRE(varDeclare.get()->getIdentifier() == "test2");
		REQUIRE(varDeclare.get()->getAssignStatemnt() != nullptr);
		REQUIRE(varDeclare.get()->getType() == TokenType::Boolean);
	}
}