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

        virtual void evaluate(Context* context) override;

		const void setColor(std::string color);
		std::string getColor() const;

	private:
		std::string colorHex;
	};
}
