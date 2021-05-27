#pragma once
#include <string>
#include "Assignable.h"

namespace AST
{
	class Color :
		public AST::Assignable
	{
	public:
		Color(std::string color);

        virtual int evaluate(Context* context) const override;

		const void setColor(std::string color);
		std::string getColor() const;

	private:
		std::string colorHex;
	};
}
