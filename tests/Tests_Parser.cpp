#include "SourceReader.h"
#include "Lexer.h"
#include "Parser.h"
#include "Token.h"
#include "TokenType.h"
#include "ProgramRootNode.h"
#include "Node.h"
#include "CallFuncStatement.h"
#include "Logger.h"
#include <iostream> 

#pragma warning(push, 0)        
#include "catch.hpp"
#pragma warning(pop)


TEST_CASE("IFStatement", "[if]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	reader->setSourceString("if (false) {} else {}");

	std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
	std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();
	REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);
	std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(firstNode);

	std::shared_ptr<Condition> condition = ifStatement->getCondition();
	REQUIRE(condition->getNodeType() == NodeType::Condition);
	REQUIRE(condition->getAndConditionSize() == 1);
	
	std::shared_ptr<AndCondition> andCondition = condition->getAndCondition();
	REQUIRE(andCondition->getNodeType() == NodeType::AndCondition);
	REQUIRE(andCondition->getRelationConditionSize() == 1);

	std::shared_ptr<RelationCondition> relationCondition = andCondition->getRelationCondition();
	REQUIRE(relationCondition->getNodeType() == NodeType::RelationCondition);
	REQUIRE(relationCondition->isBooleanWord());
	REQUIRE(!relationCondition->getBooleanWord());
	REQUIRE(!relationCondition->getNotOperator());
	REQUIRE(relationCondition->getCondition() == nullptr);
	REQUIRE(relationCondition->getExpression() == nullptr);

	
	std::shared_ptr<Node> trueBlock = ifStatement->getTrueBlockNode();
	REQUIRE(trueBlock->getNodeType() == NodeType::InstructionsBlock);
	
	std::shared_ptr<Node> elseBlock = ifStatement->getElseBlockNode();
	REQUIRE(elseBlock->getNodeType() == NodeType::InstructionsBlock);

}



TEST_CASE("RepeatStatement", "[repeat]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	reader->setSourceString("repeat(50) {}");

	std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
	std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();
	REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

	std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
	
	std::shared_ptr<Node> block = repeatStatement->getInstructuionsBlock();
	REQUIRE(block->getNodeType() == NodeType::InstructionsBlock);
	
	std::shared_ptr<Expression> expression = repeatStatement->getHowManyTime();
	REQUIRE(expression->getTermsSize() == 1);

	std::shared_ptr<TermExpression> expressionTerm = expression->getExpressionTerm(0);
	REQUIRE(expressionTerm->getFactorsSize() == 1);

	std::shared_ptr<FactorExpression> expressionFactor = expressionTerm->getExpressionFactor(0);
	REQUIRE(expressionFactor->getIntVal() == 50);
	REQUIRE(!expressionFactor->getNegativeOp());
}

TEST_CASE("RepeatTimeStatement", "[repeat]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	reader->setSourceString("repeatTime(22) {}");

	std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
	std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();
	REQUIRE(firstNode->getNodeType() == NodeType::RepeatTimeStatement);

	std::shared_ptr<RepeatTimeStatement> repeatTimeStatement = std::static_pointer_cast<RepeatTimeStatement>(firstNode);
	
	std::shared_ptr<Node> block = repeatTimeStatement->getInstructuionsBlock();
	REQUIRE(block->getNodeType() == NodeType::InstructionsBlock);
	
	std::shared_ptr<Expression> expression = repeatTimeStatement->getPeriod();
	REQUIRE(expression->getTermsSize() == 1);

	std::shared_ptr<TermExpression> expressionTerm = expression->getExpressionTerm(0);
	REQUIRE(expressionTerm->getFactorsSize() == 1);

	std::shared_ptr<FactorExpression> expressionFactor = expressionTerm->getExpressionFactor(0);
	REQUIRE(expressionFactor->getIntVal() == 22);
	REQUIRE(!expressionFactor->getNegativeOp());

	expression = repeatTimeStatement->getHowManyTime();
	REQUIRE(expression == nullptr);
}


TEST_CASE("Expressions", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("Simple expression - just digit (zero)")
	{
		reader->setSourceString("repeat(0) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement->getHowManyTime();
		REQUIRE(expression->getTermsSize() == 1);
		
		std::shared_ptr<TermExpression> expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		std::shared_ptr<FactorExpression> expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 0);
		REQUIRE(!expressionFactor->getNegativeOp());
	}
	
	SECTION("Simple expression - negative digit")
	{
		reader->setSourceString("repeat(-95) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement->getHowManyTime();
		REQUIRE(expression->getTermsSize() == 1);

		std::shared_ptr<TermExpression> expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		std::shared_ptr<FactorExpression> expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 95);
		REQUIRE(expressionFactor->getNegativeOp());
	}

	SECTION("math operation (+, -, *, /) without bracket")
	{
		reader->setSourceString("repeat(-95 + 20 * -20 / -30 - 20) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement->getHowManyTime();
		// A + B - C
		REQUIRE(expression->getTermsSize() == 3);
		REQUIRE(expression->getOperator(0) == TokenType::Plus);
		REQUIRE(expression->getOperator(1) == TokenType::Minus);

		// A Term   -95 
		std::shared_ptr<TermExpression> expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		std::shared_ptr<FactorExpression> expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 95);
		REQUIRE(expressionFactor->getNegativeOp());



		// B Term  20 * -20 / -30
		expressionTerm = expression->getExpressionTerm(1);
		REQUIRE(expressionTerm->getFactorsSize() == 3);
		REQUIRE(expressionTerm->getOperator(0) == TokenType::Multiply);
		REQUIRE(expressionTerm->getOperator(1) == TokenType::Divide);
		
		expressionFactor = expressionTerm->getExpressionFactor(0);
		// 20 * ...
		REQUIRE(expressionFactor->getIntVal() == 20);
		REQUIRE(!expressionFactor->getNegativeOp());
		
		expressionFactor = expressionTerm->getExpressionFactor(1);
		// ... * -20 / ...
		REQUIRE(expressionFactor->getIntVal() == 20);
		REQUIRE(expressionFactor->getNegativeOp());

		expressionFactor = expressionTerm->getExpressionFactor(2);
		// ... / -30 
		REQUIRE(expressionFactor->getIntVal() == 30);
		REQUIRE(expressionFactor->getNegativeOp());


		// C Term 20
		// Even its  (- 20) its not negation, its substraction
		expressionTerm = expression->getExpressionTerm(2);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 20);
		REQUIRE(!expressionFactor->getNegativeOp());
	}

	SECTION("expression with brackets")
	{
		reader->setSourceString("repeat(-(2 + -2) * 2) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement->getHowManyTime();
		REQUIRE(expression->getTermsSize() == 1);

		std::shared_ptr<TermExpression> expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 2);

		std::shared_ptr<FactorExpression> expressionFactor = expressionTerm->getExpressionFactor(1);
		REQUIRE(!expressionFactor->getNegativeOp());
		REQUIRE(expressionFactor->getIntVal() == 2);

		expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getNegativeOp());

		// bracket exprssion (2 + -2)
		std::shared_ptr<Expression> bracketExpression = expressionFactor->getExpression();
		REQUIRE(bracketExpression->getTermsSize() == 2);
		REQUIRE(bracketExpression->getOperator(0) == TokenType::Plus);
		std::shared_ptr<TermExpression> bracketTerm = bracketExpression->getExpressionTerm(0);
		REQUIRE(bracketTerm->getFactorsSize() == 1);
		std::shared_ptr<FactorExpression> bracketFactor = bracketTerm->getExpressionFactor(0);
		REQUIRE(!bracketFactor->getNegativeOp());
		REQUIRE(bracketFactor->getIntVal() == 2);
		bracketTerm = bracketExpression->getExpressionTerm(1);
		REQUIRE(bracketTerm->getFactorsSize() == 1);
		bracketFactor = bracketTerm->getExpressionFactor(0);
		REQUIRE(bracketFactor->getNegativeOp());
		REQUIRE(bracketFactor->getIntVal() == 2);
	}

	SECTION("expression fun call")
	{
		reader->setSourceString("repeat(callFunc(20)) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement->getHowManyTime();
		REQUIRE(expression->getTermsSize() == 1);

		std::shared_ptr<TermExpression> expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		std::shared_ptr<FactorExpression> expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 0);
		REQUIRE(!expressionFactor->getNegativeOp());
		REQUIRE(!expressionFactor->getExpression());
		std::shared_ptr<CallFuncStatement> callFunc = expressionFactor->getCallFunc();
		REQUIRE(callFunc->getNodeType() == NodeType::CallFuncStatement);
		REQUIRE(callFunc->getArgumentsSize() == 1);
		REQUIRE(callFunc->getIdentifiersSize() == 1);
	}

	SECTION("expression identifier")
	{
		reader->setSourceString("repeat(identifier) {}");
		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::RepeatStatement);

		std::shared_ptr<RepeatStatement> repeatStatement = std::static_pointer_cast<RepeatStatement>(firstNode);
		std::shared_ptr<Expression> expression = repeatStatement->getHowManyTime();
		REQUIRE(expression->getTermsSize() == 1);

		std::shared_ptr<TermExpression> expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		std::shared_ptr<FactorExpression> expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 0);
		REQUIRE(!expressionFactor->getNegativeOp());
		REQUIRE(!expressionFactor->getExpression());
		REQUIRE(expressionFactor->getVarName()[0] == "identifier");
	}
}



TEST_CASE("Conditions", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("simple relation Operator")
	{
		reader->setSourceString("if (2 < 3) {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();
		REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);
		std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(firstNode);

		std::shared_ptr<Condition> condition = ifStatement->getCondition();
		REQUIRE(condition->getNodeType() == NodeType::Condition);
		REQUIRE(condition->getAndConditionSize() == 1);

		std::shared_ptr<AndCondition> andCondition = condition->getAndCondition();
		REQUIRE(andCondition->getNodeType() == NodeType::AndCondition);
		REQUIRE(andCondition->getRelationConditionSize() == 1);

		std::shared_ptr<RelationCondition> relationCondition = andCondition->getRelationCondition();
		REQUIRE(relationCondition->getNodeType() == NodeType::RelationCondition);
		REQUIRE(!relationCondition->isBooleanWord());
		REQUIRE(!relationCondition->getNotOperator());
		REQUIRE(relationCondition->getCondition() == nullptr);
		REQUIRE(relationCondition->hasSecondExpression());
		REQUIRE(relationCondition->getRelationOperator() == TokenType::Less);
	}

	SECTION("AND and OR operators, not ")
	{
		reader->setSourceString("if (!true && false || true) {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();
		REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);
		std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(firstNode);

		std::shared_ptr<Condition> condition = ifStatement->getCondition();
		REQUIRE(condition->getNodeType() == NodeType::Condition);
		REQUIRE(condition->getAndConditionSize() == 2);

		std::shared_ptr<AndCondition> andCondition = condition->getAndCondition();
		REQUIRE(andCondition->getNodeType() == NodeType::AndCondition);
		REQUIRE(andCondition->getRelationConditionSize() == 2);

		std::shared_ptr<RelationCondition> relationCondition = andCondition->getRelationCondition();
		REQUIRE(relationCondition->getNodeType() == NodeType::RelationCondition);
		REQUIRE(relationCondition->isBooleanWord());
		REQUIRE(relationCondition->getBooleanWord());
		REQUIRE(relationCondition->getNotOperator());
		REQUIRE(relationCondition->getCondition() == nullptr);
		REQUIRE(!relationCondition->hasSecondExpression());
		REQUIRE(relationCondition->getExpression() == nullptr);

		relationCondition = andCondition->getRelationCondition(1);
		REQUIRE(relationCondition->getNodeType() == NodeType::RelationCondition);
		REQUIRE(relationCondition->isBooleanWord());
		REQUIRE(!relationCondition->getBooleanWord());
		REQUIRE(!relationCondition->getNotOperator());
		REQUIRE(relationCondition->getCondition() == nullptr);
		REQUIRE(!relationCondition->hasSecondExpression());
		REQUIRE(relationCondition->getExpression() == nullptr);


		andCondition = condition->getAndCondition(1);
		REQUIRE(andCondition->getNodeType() == NodeType::AndCondition);
		REQUIRE(andCondition->getRelationConditionSize() == 1);

		relationCondition = andCondition->getRelationCondition();
		REQUIRE(relationCondition->getNodeType() == NodeType::RelationCondition);
		REQUIRE(relationCondition->isBooleanWord());
		REQUIRE(relationCondition->getBooleanWord());
		REQUIRE(!relationCondition->getNotOperator());
		REQUIRE(relationCondition->getCondition() == nullptr);
		REQUIRE(!relationCondition->hasSecondExpression());
		REQUIRE(relationCondition->getExpression() == nullptr);

	}

	SECTION("Brackets")
	{
		reader->setSourceString("if (!(callFunc() || 2 == 2) && true) {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();
		REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);
		std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(firstNode);

		std::shared_ptr<Condition> condition = ifStatement->getCondition();
		REQUIRE(condition->getNodeType() == NodeType::Condition);
		REQUIRE(condition->getAndConditionSize() == 1);

		std::shared_ptr<AndCondition> andCondition = condition->getAndCondition();
		REQUIRE(andCondition->getNodeType() == NodeType::AndCondition);
		REQUIRE(andCondition->getRelationConditionSize() == 2);

		std::shared_ptr<RelationCondition> relationCondition = andCondition->getRelationCondition(1);
		REQUIRE(relationCondition->getNodeType() == NodeType::RelationCondition);
		REQUIRE(relationCondition->isBooleanWord());
		REQUIRE(relationCondition->getBooleanWord());
		REQUIRE(!relationCondition->getNotOperator());
		REQUIRE(relationCondition->getCondition() == nullptr);
		REQUIRE(!relationCondition->hasSecondExpression());
		REQUIRE(relationCondition->getExpression() == nullptr);


		relationCondition = andCondition->getRelationCondition();
		REQUIRE(relationCondition->getNodeType() == NodeType::RelationCondition);
		REQUIRE(!relationCondition->isBooleanWord());
		REQUIRE(relationCondition->getNotOperator());
		REQUIRE(relationCondition->getCondition() != nullptr);
		REQUIRE(!relationCondition->hasSecondExpression());
		REQUIRE(relationCondition->getExpression() == nullptr);

		condition = relationCondition->getCondition();
		REQUIRE(condition->getNodeType() == NodeType::Condition);
		REQUIRE(condition->getAndConditionSize() == 2);

		andCondition = condition->getAndCondition();
		REQUIRE(andCondition->getNodeType() == NodeType::AndCondition);
		REQUIRE(andCondition->getRelationConditionSize() == 1);

		relationCondition = andCondition->getRelationCondition();
		REQUIRE(relationCondition->getNodeType() == NodeType::RelationCondition);
		REQUIRE(!relationCondition->isBooleanWord());
		REQUIRE(!relationCondition->getNotOperator());
		REQUIRE(relationCondition->getCondition() == nullptr);
		REQUIRE(!relationCondition->hasSecondExpression());
		REQUIRE(relationCondition->getExpression() != nullptr);

		andCondition = condition->getAndCondition(1);
		REQUIRE(andCondition->getNodeType() == NodeType::AndCondition);
		REQUIRE(andCondition->getRelationConditionSize() == 1);

		relationCondition = andCondition->getRelationCondition();
		REQUIRE(relationCondition->getNodeType() == NodeType::RelationCondition);
		REQUIRE(!relationCondition->isBooleanWord());
		REQUIRE(!relationCondition->getNotOperator());
		REQUIRE(relationCondition->getCondition() == nullptr);
		REQUIRE(relationCondition->hasSecondExpression());
		REQUIRE(relationCondition->getExpression() != nullptr);
		REQUIRE(relationCondition->getRelationOperator() == TokenType::Equal);
	}
}

//

TEST_CASE("def function", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("simple function, without parameters, without return type")
	{
		reader->setSourceString("function test() {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::DefFuncStatement);

		std::shared_ptr<DefFuncStatement> defFunction = std::static_pointer_cast<DefFuncStatement>(firstNode);
		REQUIRE(defFunction->getName() == "test");
		REQUIRE(defFunction->getParametersSize() == 0);
		REQUIRE(!defFunction->hasReturnType());
	}

	SECTION("function with one parameters, without return type")
	{
		reader->setSourceString("function test2(Turtle zolw) {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::DefFuncStatement);

		std::shared_ptr<DefFuncStatement> defFunction = std::static_pointer_cast<DefFuncStatement>(firstNode);
		REQUIRE(defFunction->getName() == "test2");
		REQUIRE(defFunction->getParametersSize() == 1);
		REQUIRE(!defFunction->hasReturnType());

		std::shared_ptr<Parameter> parameter = defFunction->getParameter(0);
		REQUIRE(parameter->getType() == TokenType::Turtle);
		REQUIRE(parameter->getName() == "zolw");
	}

	SECTION("function with multi parameters, witout return type")
	{
		reader->setSourceString("function test3(Turtle zolw, Color color, Integer int) {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::DefFuncStatement);

		std::shared_ptr<DefFuncStatement> defFunction = std::static_pointer_cast<DefFuncStatement>(firstNode);
		REQUIRE(defFunction->getName() == "test3");
		REQUIRE(defFunction->getParametersSize() == 3);
		REQUIRE(!defFunction->hasReturnType());

		std::shared_ptr<Parameter> parameter = defFunction->getParameter(0);
		REQUIRE(parameter->getType() == TokenType::Turtle);
		REQUIRE(parameter->getName() == "zolw");
		
		parameter = defFunction->getParameter(1);
		REQUIRE(parameter->getType() == TokenType::ColorVar);
		REQUIRE(parameter->getName() == "color");

		parameter = defFunction->getParameter(2);
		REQUIRE(parameter->getType() == TokenType::Integer);
		REQUIRE(parameter->getName() == "int");
	}

	SECTION("function without parameters, with return type")
	{
		reader->setSourceString("Turtle function test4() {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::DefFuncStatement);

		std::shared_ptr<DefFuncStatement> defFunction = std::static_pointer_cast<DefFuncStatement>(firstNode);
		REQUIRE(defFunction->getName() == "test4");
		REQUIRE(defFunction->getParametersSize() == 0);
		REQUIRE(defFunction->hasReturnType());
		REQUIRE(defFunction->getReturnType() == TokenType::Turtle);
	}

	SECTION("function witn parameters, with return type")
	{
		reader->setSourceString("Integer function test5(Point point, Integer int) {}");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::DefFuncStatement);

		std::shared_ptr<DefFuncStatement> defFunction = std::static_pointer_cast<DefFuncStatement>(firstNode);
		REQUIRE(defFunction->getName() == "test5");
		REQUIRE(defFunction->getParametersSize() == 2);
		REQUIRE(defFunction->hasReturnType());
		REQUIRE(defFunction->getReturnType() == TokenType::Integer);

		std::shared_ptr<Parameter> parameter = defFunction->getParameter(0);
		REQUIRE(parameter->getType() == TokenType::Point);
		REQUIRE(parameter->getName() == "point");

		parameter = defFunction->getParameter(1);
		REQUIRE(parameter->getType() == TokenType::Integer);
		REQUIRE(parameter->getName() == "int");
	}
}

TEST_CASE("call function", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("simple call function")
	{
		reader->setSourceString("test();");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::CallFuncStatement);

		std::shared_ptr<CallFuncStatement> callFunc = std::static_pointer_cast<CallFuncStatement>(firstNode);
		REQUIRE(callFunc->getArgumentsSize() == 0);
		REQUIRE(callFunc->getIdentifiersSize() == 1);
		REQUIRE(callFunc->getIdentifier(0) == "test");
	}

	SECTION("call function with one argument, int")
	{
		reader->setSourceString("test2(153);");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::CallFuncStatement);

		std::shared_ptr<CallFuncStatement> callFunc = std::static_pointer_cast<CallFuncStatement>(firstNode);
		REQUIRE(callFunc->getArgumentsSize() == 1);
		REQUIRE(callFunc->getIdentifiersSize() == 1);
		REQUIRE(callFunc->getIdentifier(0) == "test2");


		std::shared_ptr<Expression> expression = callFunc->getArgument(0);
		REQUIRE(expression->getTermsSize() == 1);

		std::shared_ptr<TermExpression> expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		std::shared_ptr<FactorExpression> expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 153);
		REQUIRE(!expressionFactor->getNegativeOp());
	}

	SECTION("call function with more arguments")
	{
		reader->setSourceString("test3(160, -20, 10);");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::CallFuncStatement);

		std::shared_ptr<CallFuncStatement> callFunc = std::static_pointer_cast<CallFuncStatement>(firstNode);
		REQUIRE(callFunc->getArgumentsSize() == 3);
		REQUIRE(callFunc->getIdentifiersSize() == 1);
		REQUIRE(callFunc->getIdentifier(0) == "test3");


		std::shared_ptr<Expression> expression = callFunc->getArgument(0);
		REQUIRE(expression->getTermsSize() == 1);
		std::shared_ptr<TermExpression> expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);
		std::shared_ptr<FactorExpression> expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 160);
		REQUIRE(!expressionFactor->getNegativeOp());

		expression = callFunc->getArgument(1);
		REQUIRE(expression->getTermsSize() == 1);
		expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);
		expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 20);
		REQUIRE(expressionFactor->getNegativeOp());

		expression = callFunc->getArgument(2);
		REQUIRE(expression->getTermsSize() == 1);
		expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);
		expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 10);
		REQUIRE(!expressionFactor->getNegativeOp());
	}

	SECTION("call function multi level ID")
	{
		reader->setSourceString("test4.test44.test444();");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::CallFuncStatement);

		std::shared_ptr<CallFuncStatement> callFunc = std::static_pointer_cast<CallFuncStatement>(firstNode);
		REQUIRE(callFunc->getArgumentsSize() == 0);
		REQUIRE(callFunc->getIdentifiersSize() == 3);
		REQUIRE(callFunc->getIdentifier(0) == "test4");
		REQUIRE(callFunc->getIdentifier(1) == "test44");
		REQUIRE(callFunc->getIdentifier(2) == "test444");
	}

	SECTION("call function multi level ID and multi args")
	{
		reader->setSourceString("test5.test55.test555(1, -2, 3);");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::CallFuncStatement);

		std::shared_ptr<CallFuncStatement> callFunc = std::static_pointer_cast<CallFuncStatement>(firstNode);
		REQUIRE(callFunc->getArgumentsSize() == 3);
		REQUIRE(callFunc->getIdentifiersSize() == 3);
		REQUIRE(callFunc->getIdentifier(0) == "test5");
		REQUIRE(callFunc->getIdentifier(1) == "test55");
		REQUIRE(callFunc->getIdentifier(2) == "test555");

		std::shared_ptr<Expression> expression = callFunc->getArgument(0);
		REQUIRE(expression->getTermsSize() == 1);
		std::shared_ptr<TermExpression> expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);
		std::shared_ptr<FactorExpression> expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 1);
		REQUIRE(!expressionFactor->getNegativeOp());

		expression = callFunc->getArgument(1);
		REQUIRE(expression->getTermsSize() == 1);
		expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);
		expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 2);
		REQUIRE(expressionFactor->getNegativeOp());

		expression = callFunc->getArgument(2);
		REQUIRE(expression->getTermsSize() == 1);
		expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);
		expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 3);
		REQUIRE(!expressionFactor->getNegativeOp());
	
	}

}


TEST_CASE("Var declartion", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("simple def var")
	{
		reader->setSourceString("Integer test;");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::DeclareVarStatement);

		std::shared_ptr<DeclareVarStatement> varDeclare = std::static_pointer_cast<DeclareVarStatement>(firstNode);
		REQUIRE(varDeclare->getIdentifier() == "test");
		REQUIRE(varDeclare->getAssignStatemnt() == nullptr);
		REQUIRE(varDeclare->getType() == TokenType::Integer);
	}


	SECTION("def var with assign")
	{
		reader->setSourceString("Integer test2 =  2 + 2;");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::DeclareVarStatement);

		std::shared_ptr<DeclareVarStatement> varDeclare = std::static_pointer_cast<DeclareVarStatement>(firstNode);
		REQUIRE(varDeclare->getIdentifier() == "test2");
		REQUIRE(varDeclare->getAssignStatemnt() != nullptr);
		REQUIRE(varDeclare->getType() == TokenType::Integer);
	}
}

TEST_CASE("assign", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("simple assign")
	{
		reader->setSourceString("test = -20;");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::AssignStatement);

		std::shared_ptr<AssignStatement> assign = std::static_pointer_cast<AssignStatement>(firstNode);
		REQUIRE(assign->getIdentifier() == "test");

		std::shared_ptr<Expression> expression = assign->getExpression();
		REQUIRE(expression != nullptr);
		REQUIRE(expression->getTermsSize() == 1);

		std::shared_ptr<TermExpression> expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		std::shared_ptr<FactorExpression> expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 20);
		REQUIRE(expressionFactor->getNegativeOp());
	}

	SECTION("multi level id assign and calc Func")
	{
		reader->setSourceString("test2.test22.test222 = calcFunc();");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::AssignStatement);

		std::shared_ptr<AssignStatement> assign = std::static_pointer_cast<AssignStatement>(firstNode);
		REQUIRE(assign->getIdentifier() == "test2");
		REQUIRE(assign->getIdentifier(1) == "test22");
		REQUIRE(assign->getIdentifier(2) == "test222");

		std::shared_ptr<Expression> expression = assign->getExpression();
		REQUIRE(expression != nullptr);
		REQUIRE(expression->getTermsSize() == 1);

		std::shared_ptr<TermExpression> expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		std::shared_ptr<FactorExpression> expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(!expressionFactor->getNegativeOp());
		REQUIRE(expressionFactor->getCallFunc() != nullptr);
	}
}

TEST_CASE("declare class-type", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("simple declare")
	{
		reader->setSourceString("Point point; Turtle zolw;");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> node = rootNode->getNextInstruction();

		REQUIRE(node->getNodeType() == NodeType::DeclareVarStatement);
		std::shared_ptr<DeclareVarStatement> decVar = std::static_pointer_cast<DeclareVarStatement>(node);
		REQUIRE(decVar->getType() == TokenType::Point);
		REQUIRE(decVar->getAssignStatemnt() == nullptr);
		REQUIRE(decVar->getAssignClassStatement() == nullptr);
		REQUIRE(decVar->getIdentifier() == "point");

		node = rootNode->getNextInstruction();
		REQUIRE(node->getNodeType() == NodeType::DeclareVarStatement);
		decVar = std::static_pointer_cast<DeclareVarStatement>(node);
		REQUIRE(decVar->getType() == TokenType::Turtle);
		REQUIRE(decVar->getAssignStatemnt() == nullptr);
		REQUIRE(decVar->getAssignClassStatement() == nullptr);
		REQUIRE(decVar->getIdentifier() == "zolw");
	}

	SECTION("declare class with one argument")
	{
		reader->setSourceString("Point point(20); Turtle zolw(-10);");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> node = rootNode->getNextInstruction();

		std::shared_ptr<AssignClassStatement> assignClass;
		std::shared_ptr<Expression> expression;
		std::shared_ptr<TermExpression> expressionTerm;
		std::shared_ptr<FactorExpression> expressionFactor;

		REQUIRE(node->getNodeType() == NodeType::DeclareVarStatement);
		std::shared_ptr<DeclareVarStatement> decVar = std::static_pointer_cast<DeclareVarStatement>(node);
		REQUIRE(decVar->getType() == TokenType::Point);
		REQUIRE(decVar->getAssignStatemnt() == nullptr);
		REQUIRE(decVar->getAssignClassStatement() != nullptr);
		REQUIRE(decVar->getIdentifier() == "point");

		assignClass = decVar->getAssignClassStatement();
		REQUIRE(assignClass->getExpressionsSize() == 1);
		expression = assignClass->getExpression(0);
		REQUIRE(expression->getTermsSize() == 1);

		expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 20);
		REQUIRE(!expressionFactor->getNegativeOp());

		node = rootNode->getNextInstruction();
		REQUIRE(node->getNodeType() == NodeType::DeclareVarStatement);
		decVar = std::static_pointer_cast<DeclareVarStatement>(node);
		REQUIRE(decVar->getType() == TokenType::Turtle);
		REQUIRE(decVar->getAssignStatemnt() == nullptr);
		REQUIRE(decVar->getAssignClassStatement() != nullptr);
		REQUIRE(decVar->getIdentifier() == "zolw");

		assignClass = decVar->getAssignClassStatement();
		REQUIRE(assignClass->getExpressionsSize() == 1);
		expression = assignClass->getExpression(0);
		REQUIRE(expression->getTermsSize() == 1);

		expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 10);
		REQUIRE(expressionFactor->getNegativeOp());
	}

	SECTION("declare with multi args")
	{
		reader->setSourceString("Point point(20, 10, 0); Turtle zolw(-10, -20);");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> node = rootNode->getNextInstruction();

		std::shared_ptr<AssignClassStatement> assignClass;
		std::shared_ptr<Expression> expression;
		std::shared_ptr<TermExpression> expressionTerm;
		std::shared_ptr<FactorExpression> expressionFactor;

		REQUIRE(node->getNodeType() == NodeType::DeclareVarStatement);
		std::shared_ptr<DeclareVarStatement> decVar = std::static_pointer_cast<DeclareVarStatement>(node);
		REQUIRE(decVar->getType() == TokenType::Point);
		REQUIRE(decVar->getAssignStatemnt() == nullptr);
		REQUIRE(decVar->getAssignClassStatement() != nullptr);
		REQUIRE(decVar->getIdentifier() == "point");

		assignClass = decVar->getAssignClassStatement();
		REQUIRE(assignClass->getExpressionsSize() == 3);
		expression = assignClass->getExpression(0);
		REQUIRE(expression->getTermsSize() == 1);

		expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 20);
		REQUIRE(!expressionFactor->getNegativeOp());

		expression = assignClass->getExpression(1);
		REQUIRE(expression->getTermsSize() == 1);

		expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 10);
		REQUIRE(!expressionFactor->getNegativeOp());

		expression = assignClass->getExpression(2);
		REQUIRE(expression->getTermsSize() == 1);

		expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 0);
		REQUIRE(!expressionFactor->getNegativeOp());

		node = rootNode->getNextInstruction();
		REQUIRE(node->getNodeType() == NodeType::DeclareVarStatement);
		decVar = std::static_pointer_cast<DeclareVarStatement>(node);
		REQUIRE(decVar->getType() == TokenType::Turtle);
		REQUIRE(decVar->getAssignStatemnt() == nullptr);
		REQUIRE(decVar->getAssignClassStatement() != nullptr);
		REQUIRE(decVar->getIdentifier() == "zolw");

		assignClass = decVar->getAssignClassStatement();
		REQUIRE(assignClass->getExpressionsSize() == 2);
		expression = assignClass->getExpression(0);
		REQUIRE(expression->getTermsSize() == 1);

		expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 10);
		REQUIRE(expressionFactor->getNegativeOp());

		expression = assignClass->getExpression(1);
		REQUIRE(expression->getTermsSize() == 1);

		expressionTerm = expression->getExpressionTerm(0);
		REQUIRE(expressionTerm->getFactorsSize() == 1);

		expressionFactor = expressionTerm->getExpressionFactor(0);
		REQUIRE(expressionFactor->getIntVal() == 20);
		REQUIRE(expressionFactor->getNegativeOp());
	}
}

TEST_CASE("color var decl")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("simple declare")
	{
		reader->setSourceString("Color col1;");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> node = rootNode->getNextInstruction();

		REQUIRE(node->getNodeType() == NodeType::DeclareVarStatement);
		std::shared_ptr<DeclareVarStatement> decVar = std::static_pointer_cast<DeclareVarStatement>(node);
		REQUIRE(decVar->getType() == TokenType::ColorVar);
		REQUIRE(decVar->getAssignStatemnt() == nullptr);
		REQUIRE(decVar->getAssignClassStatement() == nullptr);
		REQUIRE(decVar->getIdentifier() == "col1");
		REQUIRE(decVar->getColorVal() == "");
	}

	SECTION("declare with assign")
	{
		reader->setSourceString("Color col2 = \"#123456\";");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> node = rootNode->getNextInstruction();

		REQUIRE(node->getNodeType() == NodeType::DeclareVarStatement);
		std::shared_ptr<DeclareVarStatement> decVar = std::static_pointer_cast<DeclareVarStatement>(node);
		REQUIRE(decVar->getType() == TokenType::ColorVar);
		REQUIRE(decVar->getAssignStatemnt() == nullptr);
		REQUIRE(decVar->getAssignClassStatement() == nullptr);
		REQUIRE(decVar->getIdentifier() == "col2");
		REQUIRE(decVar->getColorVal() == "#123456");
	}

}

TEST_CASE("Block of instructions", "[parser]")
{
	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	SECTION("empty block")
	{
		reader->setSourceString(R"(if (true)
{

}
)");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);

		std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(firstNode);
		std::shared_ptr<InstructionsBlock> instructionsBlock = ifStatement->getTrueBlockNode();

		REQUIRE(instructionsBlock->getInstructionsSize() == 0);
	}

	SECTION("two instructions in block")
	{
		reader->setSourceString(R"(if (true)
{
	callFunc();
	repeat(20) {}
	Turtle testTurtle;
}
)");

		std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
		std::shared_ptr<Node> firstNode = rootNode->getNextInstruction();

		REQUIRE(firstNode->getNodeType() == NodeType::IfStatement);

		std::shared_ptr<IfStatement> ifStatement = std::static_pointer_cast<IfStatement>(firstNode);
		std::shared_ptr<InstructionsBlock> instructionsBlock = ifStatement->getTrueBlockNode();

		REQUIRE(instructionsBlock->getInstructionsSize() == 3);

		std::shared_ptr<Node> instruction = instructionsBlock->getInstruction(0);
		REQUIRE(instruction->getNodeType() == NodeType::CallFuncStatement);

		instruction = instructionsBlock->getInstruction(1);
		REQUIRE(instruction->getNodeType() == NodeType::RepeatStatement);

		instruction = instructionsBlock->getInstruction(2);
		REQUIRE(instruction->getNodeType() == NodeType::DeclareVarStatement);
	}
}

TEST_CASE("Sample Code parsing", "[parser]")
{
	// A test case that checks the correct analysis of sample code that can be used in the program.
	// Includes combinations of different designs

	// Code contains: repeat keyword, digits, identifier, dot/comma/semicolon symbol, string, bracket, relation operators, mathOperators, data types 
	std::string code =
		R"(repeat(10) {
	zolw1.go(-30);
}
	 // XXXXXX COMMENT

if (x < -20) {								// XXXXXX COMMENT
	zolw.go(2 + -2);
} else {
	Integer test = 20;
}
Turtle test2;
Point test3(20, 10);
Color test4 = "#123456";
test5 = -50;
)";

	SourceReader* reader = new SourceReader();
	Lexer* lexer = new Lexer(reader);
	Logger* logger = new Logger();
	Parser parser(lexer, logger);

	reader->setSourceString(code);

	std::unique_ptr<ProgramRootNode> rootNode = parser.parse();
	REQUIRE(logger->getLogsSize() == 0);

	std::shared_ptr<Node> instruction = rootNode->getNextInstruction();

	std::shared_ptr<Expression> expression;
	std::shared_ptr<TermExpression> expressionTerm;
	std::shared_ptr<FactorExpression> expressionFactor;
	std::shared_ptr<InstructionsBlock> instructionsBlock;
	std::shared_ptr<CallFuncStatement> callFuncStatement;
	std::shared_ptr<RepeatStatement> repeatStatement;
	std::shared_ptr<IfStatement> ifStatement;
	std::shared_ptr<Condition> condition;
	std::shared_ptr<AndCondition> andCondition;
	std::shared_ptr<RelationCondition> relationCondition;
	std::shared_ptr<DeclareVarStatement> varDeclare;
	std::shared_ptr<AssignStatement> assign;
	std::shared_ptr<AssignClassStatement> assignClass;

	//////////////////////////////////
	//			REPEAT statement	//
	//////////////////////////////////
	REQUIRE(instruction->getNodeType() == NodeType::RepeatStatement);
	repeatStatement = std::static_pointer_cast<RepeatStatement>(instruction);

	// Expression - how many time repeat
	expression = repeatStatement->getHowManyTime();
	REQUIRE(expression->getTermsSize() == 1);

	expressionTerm = expression->getExpressionTerm(0);
	REQUIRE(expressionTerm->getFactorsSize() == 1);

	expressionFactor = expressionTerm->getExpressionFactor(0);
	REQUIRE(expressionFactor->getIntVal() == 10);
	REQUIRE(!expressionFactor->getNegativeOp());

	// Repeat instructions block
	instructionsBlock = repeatStatement->getInstructuionsBlock();
	REQUIRE(instructionsBlock->getInstructionsSize() == 1);

	// zolw1.go(10);
	instruction = instructionsBlock->getInstruction(0);
	REQUIRE(instruction->getNodeType() == NodeType::CallFuncStatement);
	callFuncStatement = std::static_pointer_cast<CallFuncStatement>(instruction);
	REQUIRE(callFuncStatement->getArgumentsSize() == 1);
	REQUIRE(callFuncStatement->getIdentifiersSize() == 2);
	REQUIRE(callFuncStatement->getIdentifier(0) == "zolw1");
	REQUIRE(callFuncStatement->getIdentifier(1) == "go");
	expression = callFuncStatement->getArgument(0);
	REQUIRE(expression->getTermsSize() == 1);

	expressionTerm = expression->getExpressionTerm(0);
	REQUIRE(expressionTerm->getFactorsSize() == 1);

	expressionFactor = expressionTerm->getExpressionFactor(0);
	REQUIRE(expressionFactor->getIntVal() == 30);
	REQUIRE(expressionFactor->getNegativeOp());

	//////////////////////////////////
	//			IF statement		//
	//////////////////////////////////

	instruction = rootNode->getNextInstruction();
	REQUIRE(instruction->getNodeType() == NodeType::IfStatement);
	ifStatement = std::static_pointer_cast<IfStatement>(instruction);
	REQUIRE(ifStatement->hasElseBlock());

	// IF Condition x < -20
	condition = ifStatement->getCondition();
	REQUIRE(condition->getNodeType() == NodeType::Condition);
	REQUIRE(condition->getAndConditionSize() == 1);

	andCondition = condition->getAndCondition();
	REQUIRE(andCondition->getNodeType() == NodeType::AndCondition);
	REQUIRE(andCondition->getRelationConditionSize() == 1);

	relationCondition = andCondition->getRelationCondition();
	REQUIRE(relationCondition->getNodeType() == NodeType::RelationCondition);
	REQUIRE(!relationCondition->isBooleanWord());
	REQUIRE(!relationCondition->getNotOperator());
	REQUIRE(relationCondition->getCondition() == nullptr);
	REQUIRE(relationCondition->hasSecondExpression());
	REQUIRE(relationCondition->getRelationOperator() == TokenType::Less);

	// x
	expression = relationCondition->getExpression(1);
	REQUIRE(expression->getTermsSize() == 1);

	expressionTerm = expression->getExpressionTerm(0);
	REQUIRE(expressionTerm->getFactorsSize() == 1);

	expressionFactor = expressionTerm->getExpressionFactor(0);
	REQUIRE(expressionFactor->getVarName()[0] == "x");
	REQUIRE(!expressionFactor->getNegativeOp());

	// -20
	expression = relationCondition->getExpression(2);
	REQUIRE(expression->getTermsSize() == 1);

	expressionTerm = expression->getExpressionTerm(0);
	REQUIRE(expressionTerm->getFactorsSize() == 1);

	expressionFactor = expressionTerm->getExpressionFactor(0);
	REQUIRE(expressionFactor->getIntVal() == 20);
	REQUIRE(expressionFactor->getNegativeOp());

	// if true node
	instructionsBlock = ifStatement->getTrueBlockNode();
	REQUIRE(instructionsBlock->getInstructionsSize() == 1);

	// zolw.go(2 + 2);
	instruction = instructionsBlock->getInstruction(0);
	REQUIRE(instruction->getNodeType() == NodeType::CallFuncStatement);
	callFuncStatement = std::static_pointer_cast<CallFuncStatement>(instruction);
	REQUIRE(callFuncStatement->getArgumentsSize() == 1);
	REQUIRE(callFuncStatement->getIdentifiersSize() == 2);
	REQUIRE(callFuncStatement->getIdentifier(0) == "zolw");
	REQUIRE(callFuncStatement->getIdentifier(1) == "go");

	expression = callFuncStatement->getArgument(0);
	REQUIRE(expression->getTermsSize() == 2);
	REQUIRE(expression->getOperator(0) == TokenType::Plus);

	expressionTerm = expression->getExpressionTerm(0);
	REQUIRE(expressionTerm->getFactorsSize() == 1);

	expressionFactor = expressionTerm->getExpressionFactor(0);
	REQUIRE(expressionFactor->getIntVal() == 2);
	REQUIRE(!expressionFactor->getNegativeOp());

	expressionTerm = expression->getExpressionTerm(1);
	REQUIRE(expressionTerm->getFactorsSize() == 1);

	expressionFactor = expressionTerm->getExpressionFactor(0);
	REQUIRE(expressionFactor->getIntVal() == 2);
	REQUIRE(expressionFactor->getNegativeOp());

	// if else node
	instructionsBlock = ifStatement->getElseBlockNode();
	REQUIRE(instructionsBlock->getInstructionsSize() == 1);
	
	// Integer test = 20;
	instruction = instructionsBlock->getInstruction(0);
	REQUIRE(instruction->getNodeType() == NodeType::DeclareVarStatement);
	varDeclare = std::static_pointer_cast<DeclareVarStatement>(instruction);
	REQUIRE(varDeclare->getIdentifier() == "test");
	REQUIRE(varDeclare->getAssignStatemnt() != nullptr);
	REQUIRE(varDeclare->getType() == TokenType::Integer);

	assign = varDeclare->getAssignStatemnt();
	expression = assign->getExpression();
	REQUIRE(expression != nullptr);
	REQUIRE(expression->getTermsSize() == 1);

	expressionTerm = expression->getExpressionTerm(0);
	REQUIRE(expressionTerm->getFactorsSize() == 1);

	expressionFactor = expressionTerm->getExpressionFactor(0);
	REQUIRE(expressionFactor->getIntVal() == 20);
	REQUIRE(!expressionFactor->getNegativeOp());


	//////////////////////////////////
	//			Declare Var			//
	//////////////////////////////////

	instruction = rootNode->getNextInstruction();
	// Turtle test2;
	REQUIRE(instruction->getNodeType() == NodeType::DeclareVarStatement);

	varDeclare = std::static_pointer_cast<DeclareVarStatement>(instruction);
	REQUIRE(varDeclare->getIdentifier() == "test2");
	REQUIRE(varDeclare->getAssignStatemnt() == nullptr);
	REQUIRE(varDeclare->getType() == TokenType::Turtle);
	REQUIRE(varDeclare->getAssignClassStatement() == nullptr);
	REQUIRE(varDeclare->getColorVal().empty());


	instruction = rootNode->getNextInstruction();
	//Point test3(20, 10);
	varDeclare = std::static_pointer_cast<DeclareVarStatement>(instruction);
	REQUIRE(varDeclare->getIdentifier() == "test3");
	REQUIRE(varDeclare->getAssignStatemnt() == nullptr);
	REQUIRE(varDeclare->getType() == TokenType::Point);
	REQUIRE(varDeclare->getAssignClassStatement() != nullptr);
	REQUIRE(varDeclare->getColorVal().empty());

	assignClass = varDeclare->getAssignClassStatement();
	REQUIRE(assignClass->getExpressionsSize() == 2);

	instruction = rootNode->getNextInstruction();
	// Color test4 = "#123456";
	varDeclare = std::static_pointer_cast<DeclareVarStatement>(instruction);
	REQUIRE(varDeclare->getIdentifier() == "test4");
	REQUIRE(varDeclare->getAssignStatemnt() == nullptr);
	REQUIRE(varDeclare->getType() == TokenType::ColorVar);
	REQUIRE(varDeclare->getAssignClassStatement() == nullptr);
	REQUIRE(varDeclare->getColorVal() == "#123456");

	//////////////////////////////////
	//			assign Var			//
	//////////////////////////////////
	instruction = rootNode->getNextInstruction();
	// test5 = -50;
	REQUIRE(instruction->getNodeType() == NodeType::AssignStatement);
	assign = std::static_pointer_cast<AssignStatement>(instruction);

	expression = assign->getExpression();
	REQUIRE(expression != nullptr);
	REQUIRE(expression->getTermsSize() == 1);

	expressionTerm = expression->getExpressionTerm(0);
	REQUIRE(expressionTerm->getFactorsSize() == 1);

	expressionFactor = expressionTerm->getExpressionFactor(0);
	REQUIRE(expressionFactor->getIntVal() == 50);
	REQUIRE(expressionFactor->getNegativeOp());
}