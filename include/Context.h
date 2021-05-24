#pragma once
#include <vector>
#include <string>
#include "Node.h"
#include "DefFuncStatement.h"




class Context
{
public:
	void addDefFunction(AST::DefFuncStatement* defFunction);

	const bool isDefFunction(std::string name);

private:
	std::vector<AST::DefFuncStatement*> defFunctions;
};

