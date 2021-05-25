#pragma once
#include <vector>
#include "Expression.h"


namespace AST 
{
	class CallFuncStatement :
		public AST::Expression
	{
	public:
		CallFuncStatement();

		virtual void execute(Context* context);

		const void addArgument(std::unique_ptr<Expression> arg);
		const void addIdentifier(const std::string id);
		const void addIdentifiers(const std::vector<std::string>& ids);

		const std::size_t getArgumentsSize() const;
		Expression* getArgument(int index) const;
		const std::size_t getIdentifiersSize() const;
		std::string getIdentifier(int index) const;

	private:
		std::vector<std::unique_ptr<Expression>> arguments;
		std::vector<std::string> identifiers;
	};
}