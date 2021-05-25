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

		const void setColor(std::string color);
		std::string getColor() const;

	private:
		std::string colorHex;
	};
}