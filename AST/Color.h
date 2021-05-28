#pragma once
#include <string>
#include "Expression.h"

namespace AST
{
	class Color :
        public AST::Expression
	{
	public:
		Color(std::string color);

        virtual void evaluate(Context* context) override;

		const void setColor(std::string color);
		std::string getColor() const;

	private:
		std::string colorHex;
	};
}
